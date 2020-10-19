#include "cub.h"

/*
**	wall_spot[0] = which face (in char)
**	wall_spot[1] = x position of the ray on this wall
*/

static int	**get_texture_infos(t_params params, t_print_vars *vars)
{
	int		**texture;

	if (vars->face == 'N')
		texture = params.textures.north;
	else if (vars->face == 'S')
		texture = params.textures.south;
	else if (vars->face == 'E')
		texture = params.textures.east;
	else
		texture = params.textures.west;
	vars->tex_size[0] = 0;
	vars->tex_size[1] = 0;
	while (texture[vars->tex_size[0]])
		vars->tex_size[0]++;
	while (texture[vars->tex_size[0] - 1][vars->tex_size[1]] > -1)
		vars->tex_size[1]++;
	return (texture);
}


static void print_surfaces(int x, t_params params, t_print_vars vars)
{
	int		i;
	int		**texture;
	float	tex_spot;

	texture = get_texture_infos(params, &vars);
	vars.tex_pos[0] = (int)(vars.wall_x * (float)vars.tex_size[0]);
	vars.step = (float)vars.tex_size[1] / (float)vars.line_height;
	tex_spot = (vars.start - params.window_xy[1] / 2 + vars.line_height / 2) * vars.step;
	i = 0;
	while (i < vars.start)
		mlx_pixel_put(params.mlx_ptr, params.win_ptr, x, i++, params.ceiling_rgb);
	while (i < vars.end)
	{
		vars.tex_pos[1] = (int)tex_spot & (vars.tex_size[0] - 1);
		tex_spot += vars.step;
		mlx_pixel_put(params.mlx_ptr, params.win_ptr, x, i++, texture[vars.tex_pos[0]][vars.tex_pos[1]]);
	}
	while (i < params.window_xy[1])
		mlx_pixel_put(params.mlx_ptr, params.win_ptr, x, i++, params.floor_rgb);
}

static bool	put_line(int x, t_params params, float **buffer, t_print_vars *vars)
{
	float			wall_dist;

	wall_dist = cub_cast_ray(2 * (float)x / params.window_xy[0] - 1, params, vars);
	if (wall_dist == -1)
		return (false);
	vars->line_height = (int)(params.window_xy[1] / wall_dist);
	vars->start = -vars->line_height / 2 + params.window_xy[1] / 2;
	if (vars->start < 0)
		vars->start = 0;
	vars->end = vars->line_height / 2 + params.window_xy[1] / 2;
	if (vars->end > params.window_xy[1] || vars->end < 0)
		vars->end = params.window_xy[1];
	print_surfaces(x, params, *vars);
	buffer[0][x] = wall_dist;
	return (true);
}

static void		sort_list(float **tab)
{
	int		i;
	float	dist[2];
	float	temp[2];

	if (tab && tab[0])
	{
		i = 0;
		while (tab[i + 1])
		{
			dist[0] = sqrt(tab[i][0] * tab[i][0] + tab[i][1] * tab[i][1]);
			dist[1] = sqrt(tab[i + 1][0] * tab[i + 1][0]
				+ tab[i + 1][1] * tab[i + 1][1]);
			if (dist[0] > dist[1])
			{
				temp[0] = tab[i][0];
				temp[1] = tab[i][1];
				tab[i][0] = tab[i + 1][0];
				tab[i][1] = tab[i + 1][1];
				tab[i + 1][0] = temp[0];
				tab[i + 1][1] = temp[1];
				i = -1;
			}
			i++;
		}
		
	}
}

int			cub_put_map(t_params params)
{
	int				x;
	t_print_vars	vars;
	bool			ret;
	float			*buffer;

	if (!(buffer = (float *)malloc(sizeof(float) * params.window_xy[0])))
		return (-1);
	vars.rel_sprites_pos = NULL;
	x = 0;
	ret = true;
	while (x < params.window_xy[0] && ret == true)
		ret = put_line(x++, params, &buffer, &vars);
	if (ret == false)
	{
		free(buffer);
		return (-1);
	}
	sort_list(vars.rel_sprites_pos);
	cub_print_sprites(params, vars, buffer);
	free(buffer);
	return (0);
}