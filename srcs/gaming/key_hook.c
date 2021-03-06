#include "cub.h"

static void	rotate_player(int dir, t_params *params)
{
	float	old_dir_x;
	float	old_plane_x;

	old_dir_x = params->player.ori_x;
	params->player.ori_x *= cos(ROTATE_SPEED * dir);
	params->player.ori_x -= params->player.ori_y * sin(ROTATE_SPEED * dir);
	params->player.ori_y *= cos(ROTATE_SPEED * dir);
	params->player.ori_y += old_dir_x * sin(ROTATE_SPEED * dir);
	old_plane_x = params->player.cam_x;
	params->player.cam_x *= cos(ROTATE_SPEED * dir);
	params->player.cam_x -= params->player.cam_y * sin(ROTATE_SPEED * dir);
	params->player.cam_y *= cos(ROTATE_SPEED * dir);
	params->player.cam_y += old_plane_x * sin(ROTATE_SPEED * dir);
}

int			cub_key_press_hook(int key, t_params *params)
{
	if (key == K_ARROW_RIGHT)
	{
		rotate_player(1, params);
		cub_put_map(*params);
	}
	else if (key == K_ARROW_LEFT)
	{
		rotate_player(-1, params);
		cub_put_map(*params);
	}
	else if (key == FORWARD || key == BACKWARD || key == RIGHT || key == LEFT)
	{
		cub_move_player(key, params);
		cub_put_map(*params);
	}
	return (0);
}

int			cub_key_release_hook(int key, t_params *params)
{
	if (key == K_ESCAPE)
	{
		cub_free_params(*params);
		exit(EXIT_SUCCESS);
	}
	return (0);
}
