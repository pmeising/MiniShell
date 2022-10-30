NAME = minishell
SRC_DIR = src/
UT_DIR	= utils/
OBJ_DIR = obj/
INC_DIR = inc/
LIBFT_DIR = libft/
LIBFT_EXEC = libft.a
READLINE	=  -I ${HOME}/goinfre/.brew/opt/readline/include/ -L ${HOME}/goinfre/.brew/opt/readline/lib/ -lreadline

SRC =	${SRC_DIR}main.c \
		${SRC_DIR}ft_env.c \
		${SRC_DIR}init_minishell.c \
		${SRC_DIR}env_utils.c \
		${SRC_DIR}signals.c \
		${SRC_DIR}validity_checks.c \
		${SRC_DIR}validity_checks_1.c \
		${SRC_DIR}validity_checks_2.c \
		${SRC_DIR}validity_utils.c \
		${SRC_DIR}ft_delim_funcs.c \
		${SRC_DIR}ft_lst_funcs.c \
		${SRC_DIR}ft_parsing.c \
		${SRC_DIR}ft_print_funcs.c \
		${SRC_DIR}check_builtin.c \
		${SRC_DIR}check_builtin2.c \
		${SRC_DIR}ft_free_funcs.c \
		${SRC_DIR}ft_interpret.c \
		${SRC_DIR}parsing_utils.c \
		${SRC_DIR}ft_env_vars_funcs.c \
		${SRC_DIR}ft_built_in_exec.c \
		

# takes the src files and converts them into .o files and puts them into /obj
# % means "take all" (=wildcard)
OBJ = $(SRC:$(SRC_DIR)%.c=$(OBJ_DIR)%.o)

# compiling flags
# -g gives debugging information when using vaglgrind
CC = gcc
CFLAGS = -Wall -Wextra -Werror -g -I $(INC_DIR)

# makefile starts here
all: $(NAME)

# create obj directory ($@ represents name of target/rule)
$(OBJ_DIR):
			@mkdir $@
			
# before OBJ can be executed an /obj directory has to be created	
$(OBJ): | $(OBJ_DIR)

# compiles .c files into .o files and puts them into /obj directory ($< represents first prerequisite required) 
$(OBJ): $(OBJ_DIR)%.o: $(SRC_DIR)%.c
		$(CC) $(CFLAGS) -c $< -o $@
		
# compiles the object files and creates the executable
# $^ : name of prerequisite/dependency that caused the rule/target (=whole thing) to execute
$(NAME):	$(OBJ)
			$(MAKE) bonus -C libft
			$(CC) $(CFLAGS) $(LIBFT_DIR)$(LIBFT_EXEC) ${READLINE} $^ -o $@
			
# remove all object files and also clean libft
clean:
		$(MAKE) fclean -C libft
		rm -f $(OBJ)
		rmdir $(OBJ_DIR)

# same as clean but also removes the executable
fclean: clean
	$(MAKE) fclean -C libft
		rm -f $(NAME)

# make the whole compilation from scratch
re: fclean all

# prevents rules and files with potentially same names being mixed up
.PHONY = all clean fclean re

