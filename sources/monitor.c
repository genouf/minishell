#include "minishell.h"

static void	malloc_failed(void)
{
	ft_putstr_fd("Malloc failed !", 2);
	rl_clear_history();
	exit(500);
}

static void	parsing_part(t_list **venv, t_list **token_list, t_list **cmd, char *input)
{
	*token_list = generate_token(input, 1);
	expanser(token_list, *venv, 0);
	if (parser_checker(token_list))
		return (ft_lstclear(venv, &del_venv), ft_lstclear(token_list, &del_token_lex), exit(2));
	*cmd = generate_cmd(*token_list, *venv);
	if (*cmd == NULL)
	{
		printf("ERROR\n");
		printf("%s\n", strerror(errno));
		exit(1);
	}
}

void	monitor(t_list *venv, t_list *token_list, t_list *cmd, char **env)
{
	char	*input;

	if (parse_env_data(&venv, env))
		return (malloc_failed());
	while (42)
	{
		//printf("isatty : %d\n", isatty(STDIN_FILENO));
		input = readline("MINISHELL$> ");
		if (input == 0)
			return (ft_lstclear(&venv, &del_venv), printf("exit\n"), rl_clear_history(), exit(g_status));
		add_history(input);
		parsing_part(&venv, &token_list, &cmd, input);
		//print_cmd(cmd);
		exec(cmd, venv); //child non ferme lors de l'ajout d'un infile
		ft_lstclear(&cmd, &del_cmd);
		free(input);
	}
}
