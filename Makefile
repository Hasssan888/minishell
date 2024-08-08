# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: aelkheta <aelkheta@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/05/13 14:40:15 by aelkheta          #+#    #+#              #
#    Updated: 2024/08/08 22:26:58 by aelkheta         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

MAKEFLAGS += --no-print-directory
NAME = minishell
CC = cc
CFLAGS = -Wall -Wextra -Werror -g3 #-fsanitize=address
RLFLAG = -lreadline
SRCDIR = ./srcs/parsing_src/
BLTDIR = ./srcs/builtin_src/
EXECDIR = ./exec
LIBFT = ./libraries/libft/libft.a

OBJDIR = ./objs

SRC = 	$(SRCDIR)minishell.c\
		$(SRCDIR)cleanup.c\
		$(SRCDIR)parsing.c\
		$(SRCDIR)parsing_utiles.c\
		$(SRCDIR)ft_split_str.c\
		$(SRCDIR)mini_utiles1.c\
		$(SRCDIR)mini_utiles2.c\
		$(SRCDIR)mini_utiles3.c\
		$(SRCDIR)exp_utiles1.c\
		$(SRCDIR)exp_utiles2.c\
		$(SRCDIR)exp_utiles3.c\
		$(SRCDIR)exp_utiles4.c\
		$(SRCDIR)tokenizer.c\
		$(SRCDIR)expander.c\
		$(SRCDIR)lexer.c\
		$(SRCDIR)print_functions.c\
		$(BLTDIR)built_in_.c\
		$(BLTDIR)echo_.c\
		$(BLTDIR)exit_.c\
		$(BLTDIR)export_utls.c\
		$(BLTDIR)cd_.c\
		$(BLTDIR)env_.c\
		$(BLTDIR)export_.c\
		$(BLTDIR)pwd_unset.c\
		$(EXECDIR)/allcmd.c\
		$(EXECDIR)/error.c\
		$(EXECDIR)/excute.c\
		$(EXECDIR)/here_doc.c\
		$(EXECDIR)/link_list.c\
		$(EXECDIR)/main.c\
		$(EXECDIR)/strjoin.c\
		$(EXECDIR)/one_cmd.c\
		$(EXECDIR)/open_file.c\
		$(EXECDIR)/utillis.c\

OBJ = $(patsubst $(SRCDIR)/%.c,$(OBJDIR)/%.o,$(SRC)) # for pathern substitution // $(patsubst pattern,replacement,text)


all: $(NAME)

$(OBJDIR)/%.o: $(SRCDIR)/%.c
	@mkdir -p $(OBJDIR)
	@$(CC) $(CFLAGS) -c $< -o $@

# $(LIBFT):
# 	@echo "Compiling libft..."
# 	@make -C ./libraries/libft
# 	@make bonus -C ./libraries/libft
# 	@echo "Compiling libft done"

# $(NAME): $(LIBFT) $(OBJ)
$(NAME): $(OBJ)
	@echo "Building..."
	@$(CC) $(CFLAGS) $(OBJ) $(RLFLAG) $(LIBFT) -o $@
	@echo "Building done"

clean:
	@echo "Removing libft obj files..."
	# @make clean -C ./libraries/libft
	@echo "Removing libft obj files..."
	@echo "Removing object files..."
	@rm -rf $(OBJDIR)/*.o
	@echo "Removing object files done"

fclean: clean
	# @make fclean -C ./libraries/libft
	@echo "Removing program name..."
	@rm -f $(NAME)
	@echo "Removing program name done"

re: fclean all

.PHONY: all clean fclean re
