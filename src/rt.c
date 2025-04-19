/* rt.c - C implementation of the ray tracer of Peter Shirley's books

   Copyright (C) 2022 Jean-Baptiste Loutfalla <jb.loutfalla@orange.fr>

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program. If not, see <http://www.gnu.org/licenses/>. */

#include <stdint.h>
#include <string.h>
#include <time.h>
#include <pthread.h>

#include "utils.h"
#include "image.h"
#include "camera.h"

#include "materials/lambertian.h"
#include "materials/metal.h"
#include "materials/dielectric.h"

#include "hittables/sphere.h"

#ifndef MAXTHREAD
#define MAXTHREAD 24
#endif /* MAXTHREAD */

#ifdef THREADED
#undef THREADED
#define THREADED 1
#else
#define THREADED 0
#endif /* THREADED */

void    *thread_compute (void *arg);
void     random_scene (world_t *world, material_list *mat_list);

static uint8_t *thread_buffers[MAXTHREAD] = {0};

typedef struct
{
  size_t buffer_size;
  world_t *world;
  material_list *mat_list;
  camera cam;

  int thread_index;
  int width;
  int height;
  int samples;
  int depth;
  int line;
} rt_thread_t;

int
main (void)
{
  srand (time (NULL));

  /* Image */
  const double aspect_ratio = 3.0 / 2.0;
  const int width = 1200;
  const int height = (int)(width / aspect_ratio);
  const int samples = 500;
  const int max_depth = 50;

  /* Materials list & World */
  material_list *mat_list = material_list_init ();
  world_t *world = world_init ();

  random_scene (world, mat_list);

  /* Camera */
  point lookfrom = vec3_init (13.0, 2.0, 3.0);
  point lookat   = vec3_init ( 0.0, 0.0, 0.0);
  vec3 vup       = vec3_init ( 0.0, 1.0, 0.0);
  double dist_to_focus = 10.0;
  double aperture = 0.1;

  camera cam = camera_init (lookfrom, lookat, vup, 20, aspect_ratio, aperture, dist_to_focus);

  /* Render */
  if (THREADED)
    {
      int nb_threads = MAXTHREAD;
      while (nb_threads > 0 && height % nb_threads != 0)
        nb_threads--;

      if (nb_threads == 0)
        nb_threads = 1;

      printf ("%dx%d\n", width, height);
      printf ("Creating %d threads\n", nb_threads);
      pthread_t *threads = (pthread_t *)malloc (nb_threads * sizeof (*threads));

      const int pixel_line = width * 3;
      const int nb_lines = height / nb_threads;
      const size_t buff_size = nb_lines * pixel_line;

      for (int i = 0; i < nb_threads; ++i)
        {
          rt_thread_t *arg = (rt_thread_t *)malloc (sizeof (rt_thread_t));
          arg->buffer_size = buff_size;
          arg->world = world;
          arg->mat_list = mat_list;
          arg->cam = cam;
          arg->thread_index = i;
          arg->width = width;
          arg->height = height;
          arg->samples = samples;
          arg->depth = max_depth;
          arg->line = i * nb_lines;

          pthread_create (threads + i, NULL, thread_compute, (void *)arg);
        }

      for (int i = 0; i < nb_threads; ++i)
        pthread_join (threads[i], NULL);

      free (threads);

      uint8_t *image = (uint8_t *)malloc (width * height * 3);
      uint8_t *img_iter = image;
      for (int i = 0; i < nb_threads; ++i)
        {
          memcpy ((void *)img_iter, (void *)thread_buffers[i], buff_size);
          free ((void *)thread_buffers[i]);
          img_iter += buff_size;
        }

      save_png ("out.png", width, height, image);
      free (image);
    }
  else
    {
      FILE *out = stdout;
      int i, j, s;
      color pixel;

      fprintf (out, "P3\n%d %d\n255\n", width, height);
      for (j = height - 1; j >= 0; --j)
        {
          fprintf (stderr, "\rScanlines remaining: %d ", j);
          fflush (stderr);

          for (i = 0; i < (int)width; ++i)
            {
              pixel = vec3_zero ();
              for (s = 0; s < samples; ++s)
                {
                  double u = (i + randd01 ()) / (double)(width - 1);
                  double v = (j + randd01 ()) / (double)(height - 1);
                  ray r = camera_get_ray (cam, u, v);
                  pixel = vec3_add (pixel, ray_color (r, mat_list, world, max_depth));
                }
              write_color (out, pixel, samples);
            }
        }

      fprintf (stderr, "\n");
    }
  world_release (world);
  material_list_release (mat_list);

  return 0;
}

void *
thread_compute (void *arg)
{
  rt_thread_t *data = (rt_thread_t *)arg;
  if (data == NULL)
    return NULL;

  int thread_index = data->thread_index;
  size_t buffer_size = data->buffer_size;
  uint8_t *pixel_buffer = (uint8_t *)malloc (buffer_size);
  thread_buffers[thread_index] = pixel_buffer;

  world_t *world = data->world;
  material_list *mat_list = data->mat_list;
  camera cam = data->cam;
  int width = data->width;
  int height = data->height;
  int samples = data->samples;
  int max_depth = data->depth;

  int j = height - 1 - data->line;

  color pixel;
  size_t offset = 0;
  while (offset < buffer_size)
    {
      for (int i = 0; i < width; ++i)
        {
          uint8_t *tmp = pixel_buffer;
          pixel = vec3_zero ();
          for (int s = 0; s < samples; ++s)
            {
              double u = (i + randd01 ()) / (double)(width - 1);
              double v = (j + randd01 ()) / (double)(height - 1);
              ray r = camera_get_ray (cam, u, v);
              pixel = vec3_add (pixel, ray_color (r, mat_list, world, max_depth)); 
            }
          pixel_buffer = write_pixel (pixel_buffer, pixel, samples);
          offset += (pixel_buffer - tmp);
        }
      j--;
    }

  free (arg);
  return NULL;
}

void random_scene (world_t *world, material_list *mat_list)
{
  int a, b, sphere_mat_idx;
  double choose_mat;
  point center;
  const point min_offset = vec3_init (4.0, 0.2, 0.0);
  material sphere_mat = { 0 };

  lambertian ground = lambertian_create (vec3_init (0.5, 0.5, 0.5));
  int ground_idx = material_list_add (mat_list, ground);
  world_add(world, sphere_create(vec3_init (0.0, -1000.0, 0.0), 1000.0, ground_idx));

  for (a = -11; a < 11; ++a)
    {
      for (b = -11; b < 11; ++b)
        {
          choose_mat = randd01 ();
          center = vec3_init (a + 0.9 * randd01 (),
                              0.2,
                              b + 0.9 * randd01 ());

          if (vec3_length (vec3_sub (center, min_offset)) <= 0.9)
            continue;

          if (choose_mat < 0.8)
            {
              /* Diffuse */
              color albedo = vec3_mult (vec3_random01 (), vec3_random01 ());
              sphere_mat = lambertian_create (albedo);
            }
          else if (choose_mat < 0.95)
            {
              /* Metal */
              color albedo = vec3_random (0.5, 1.0);
              double fuzz = randd (0.0, 0.5);
              sphere_mat = metal_create (albedo, fuzz);
            }
          else
            {
              /* Glass */
              sphere_mat = dielectric_create (1.5);
            }

          sphere_mat_idx = material_list_add (mat_list, sphere_mat);
          world_add (world, sphere_create (center, 0.2, sphere_mat_idx));
        }
    }

  {
    sphere_mat = dielectric_create (1.5);
    sphere_mat_idx = material_list_add (mat_list, sphere_mat);
    world_add (world, sphere_create (vec3_init (0.0, 1.0, 0.0), 1.0, sphere_mat_idx));
  }

  {
    sphere_mat = lambertian_create (vec3_init (0.4, 0.2, 0.1));
    sphere_mat_idx = material_list_add (mat_list, sphere_mat);
    world_add (world, sphere_create (vec3_init (-4.0, 1.0, 0.0), 1.0, sphere_mat_idx));
  }

  {
    sphere_mat = metal_create (vec3_init (0.7, 0.6, 0.5), 0.0);
    sphere_mat_idx = material_list_add (mat_list, sphere_mat);
    world_add (world, sphere_create (vec3_init (4.0, 1.0, 0.0), 1.0, sphere_mat_idx));
  }
}
