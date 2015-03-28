/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_draw.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bsautron <bsautron@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2015/01/20 20:32:16 by bsautron          #+#    #+#             */
/*   Updated: 2015/03/28 21:53:45 by bsautron         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf3d.h"

#define POS		int x, int y

static void	ft_plafond(t_env *env, int x, double h)
{
	double		plafond;

	plafond = 0;
	while (plafond < env->mid - h / 2)
	{
		env->img.char_img[x * 4 + (int)plafond * env->img.size_line] = 70;
		env->img.char_img[x * 4 + (int)plafond * env->img.size_line + 1] = 70;
		env->img.char_img[x * 4 + (int)plafond * env->img.size_line + 2] = 70;
		plafond++;
	}
}

static void	ft_floor(t_env *env, int x, double h)
{
	double		sol;
	double		y_p;

	sol = HEIGHT - 1;
	y_p = 0;
	while (sol > env->mid + h / 2)
	{
		env->img.char_img[x * 4 + (int)sol * env->img.size_line] = 50;
		env->img.char_img[x * 4 + (int)sol * env->img.size_line + 1] = 50;
		env->img.char_img[x * 4 + (int)sol * env->img.size_line + 2] = 50;
		y_p++;
		sol--;
	}
}

static void	ft_pt(t_env *env, POS, int y_p, int the_x)
{
	int			nuance;

	nuance = 0;
	if (env->vorh == 'h' && env->alpha >= 0 && env->alpha < 180)
		nuance = 0x0066FF;
	else if (env->vorh == 'h' && env->alpha >= 180)
		nuance = 0xAA6699;
	else if (env->vorh == 'v' && env->alpha >= 90 && env->alpha < 270)
		nuance = 0xFF0033;
	else if (env->vorh == 'v' && (env->alpha < 90 || env->alpha >= 270))
		nuance = 0xFFFF00;
	env->img.char_img[x * 4 + (int)y * env->img.size_line] =
		env->pic[0].char_img[(int)the_x * 4 + (int)y_p
		* env->pic[0].size_line] + 0.1 * (unsigned char)nuance;
	env->img.char_img[x * 4 + (int)y * env->img.size_line + 1] =
		env->pic[0].char_img[(int)the_x * 4 + (int)y_p
		* env->pic[0].size_line + 1] + 0.1 * (unsigned char)(nuance >> 8);
	env->img.char_img[x * 4 + (int)y * env->img.size_line + 2] =
		env->pic[0].char_img[(int)the_x * 4 + (int)y_p
		* env->pic[0].size_line + 2] + 0.1 * (unsigned char)(nuance >> 16);
}

static void	ft_mursup(t_env *env, int x, double h, int the_x)
{
	double		y;
	double		y_p;

	y = env->mid;
	y_p = env->pic[0].height / 2;
	while (y >= 0 && y - env->mid > -h / 2)
	{
		if (y_p < 0)
			y_p = 0;
		ft_pt(env, x, y, y_p, the_x);
		y_p -= env->pic[0].height / h;
		y--;
	}
}

static void	ft_murinf(t_env *env, int x, double h, int the_x)
{
	double		y;
	double		y_p;

	y = env->mid;
	y_p = env->pic[0].height / 2;
	while (y <= env->h_win - 1 && y - env->mid < h / 2)
	{
		if (y_p > env->pic[0].height - 1)
			y_p = env->pic[0].height - 1;
		ft_pt(env, x, y, y_p, the_x);
		y_p += env->pic[0].height / h;
		y++;
	}
}

static void	ft_arme(t_env *env)
{
	int		i;
	int		j;
	double	y;
	double	x;
	int		trans;

	i = HEIGHT / 2 - 40;
	y = 0;
	trans = env->pic[1].char_img[0]; 
	while (y < env->pic[1].height && i < HEIGHT - 1)
	{
		j =  WIDTH / 2 - (env->pic[1].width / (2 * 500 / (double)env->h_win)) / 2 - 5;
		x = 0;
		while (x < env->pic[1].width && j < WIDTH - 1)
		{
			if (env->pic[1].char_img[(int)x * 4 + (int)y * env->pic[1].size_line] != trans)
			{
			env->img.char_img[j * 4 + i * env->img.size_line] = 
				env->pic[1].char_img[(int)x * 4 + (int)y * env->pic[1].size_line];
			env->img.char_img[j * 4 + i * env->img.size_line + 1] =
				env->pic[1].char_img[(int)x * 4 + (int)y * env->pic[1].size_line + 1];
			env->img.char_img[j * 4 + (int)i * env->img.size_line + 2] =
				env->pic[1].char_img[(int)x * 4 + (int)y * env->pic[1].size_line + 1];
			}
			x += 2 * 500 / fmin((double)env->h_win, (double)env->w_win);
			j++;
		}
		y += 2 * 500 / fmin((double)env->h_win, (double)env->w_win);
		i++;
	}
}

void		ft_draw(t_env *env)
{
	size_t		x;
	double		d;
	int			the_mur_x;

	env->alpha = env->bruno.angle + 30;
	if (env->input.accu)
	{
		env->i_angle = 40.0 / (double)env->w_win;
		env->alpha -= 10;
	}
	else
		env->i_angle = 60.0 / (double)env->w_win;
	x = 0;
	while (x < env->w_win)
	{
		if (env->alpha >= 360)
			env->alpha -= 360;
		if (env->alpha < 0)
			env->alpha += 360;
		d = ft_calcul(env, env->alpha);
		d = d * cos(ft_dtor(env->bruno.angle - env->alpha));
		env->h = env->sz_wall / d * env->bruno.d_proj;
		if (env->input.accu)
			env->h *= 1.5;
		if (env->vorh == 'h')
			the_mur_x = ((int)env->hit.x
					* env->pic[0].width / env->sz_wall) % (int)env->pic[0].width;
		else
			the_mur_x = ((int)env->hit.y
					* env->pic[0].width / env->sz_wall) % (int)env->pic[0].width;
		ft_plafond(env, x, env->h);
		ft_murinf(env, x, env->h, the_mur_x);
		ft_mursup(env, x, env->h, the_mur_x);
		ft_floor(env, x, env->h);
		env->alpha = env->alpha - env->i_angle;
		x++;
	}
	if (env->input.accu)
		ft_arme(env);
}
