/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   main.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: ngerrets <ngerrets@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/02/01 16:05:22 by ngerrets      #+#    #+#                 */
/*   Updated: 2022/02/09 14:36:33 by ngerrets      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

static int	program_free(t_program *program)
{
	free(program->philos);
	free(program->threads);
	free(program->forks);
	return (ERROR);
}

static int	program_initialize(t_program *program)
{
	program->philos = malloc(program->input.nphilo * sizeof(t_philo));
	if (program->philos == NULL)
		return (ERROR);
	program->forks = malloc(program->input.nphilo * sizeof(pthread_mutex_t));
	if (program->forks == NULL)
		return (program_free(program));
	program->threads = malloc(program->input.nphilo * sizeof(pthread_t));
	if (program->threads == NULL)
		return (program_free(program));
	if (mutexes_init(program) == ERROR)
		return (program_free(program));
	program->start_time = time_get();
	return (SUCCES);
}

static int	program_exit(t_program *program, int exit_status)
{
	mutexes_destroy(program);
	program_free(program);
	return (exit_status);
}

static void	_bzero(void *dst, size_t size)
{
	unsigned char	*d;

	d = dst;
	while (size > 0)
	{
		*d = 0;
		d++;
		size--;
	}
}

int	main(int argc, char **argv)
{
	t_program	program;

	_bzero(&program, sizeof(t_program));
	if (parse_input(&program.input, argc, argv) == ERROR)
		return (ERROR);
	if (program_initialize(&program) == ERROR)
		return (ERROR);
	if (threads_create(&program) == ERROR)
		return (program_exit(&program, ERROR));
	threads_join(&program);
	return (program_exit(&program, SUCCES));
}
