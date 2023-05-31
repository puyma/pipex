# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: mpuig-ma <mpuig-ma@student.42barcel>       +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/04/21 20:00:01 by mpuig-ma          #+#    #+#              #
#    Updated: 2023/05/31 17:26:24 by mpuig-ma         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

AUTHOR		?=	mpuig-ma

BUILD_DIR	:=	build
SRC_DIR		:=	src
LIBFT_DIR	:=	$(SRC_DIR)/libft

LIBFT		:=	$(LIBFT_DIR)/libft.a
NAME		:=	pipex

CC			:=	gcc
CFLAGS		:=	-Wall -Werror -Wextra
CFLAGS		+=	-MMD
LFLAGS		:=	-L./$(LIBFT_DIR) -lft
DEBUG		:=	-g -fsanitize='address,undefined'
#CFLAGS		+=	$(DEBUG)
INC			:=	-I$(SRC_DIR) -I$(LIBFT_DIR)/src
RM			:=	rm -rf

OS			:=	$(shell uname)
#ifeq ($(OS),Linux)
#endif

NOSTYLE		:=	\033[0m
STYLE		:=	\033[0;32m

SRC_FILES	:=	$(SRC_DIR)/main.c \
				$(SRC_DIR)/ft_execvpe.c \
				$(SRC_DIR)/ft_getenv.c \
				$(SRC_DIR)/ft_which.c

BONUS_FILES	:=	$(SRC_DIR)/main_bonus.c \
				$(SRC_DIR)/utils_bonus.c \
				$(SRC_DIR)/ft_execvpe.c \
				$(SRC_DIR)/ft_getenv.c \
				$(SRC_DIR)/ft_which.c

OBJ_FILES	=	$(addprefix $(BUILD_DIR)/, $(addsuffix .o, $(notdir $(basename $(SRC_FILES)))))
DEP_FILES	=	$(addprefix $(BUILD_DIR)/, $(addsuffix .d, $(notdir $(basename $(SRC_FILES)))))

BOBJ_FILES	=	$(addprefix $(BUILD_DIR)/, $(addsuffix .o, $(notdir $(basename $(BONUS_FILES)))))
BDEP_FILES	=	$(addprefix $(BUILD_DIR)/, $(addsuffix .d, $(notdir $(basename $(BONUS_FILES)))))

.PHONY: clean fclean re all debug

$(NAME): $(LIBFT) $(OBJ_FILES) $(DEP_FILES) $(SRC_DIR)/$(NAME).h
	$(CC) $(INC) $(CFLAGS) $(LFLAGS) $(OBJ_FILES) -o $(NAME)
	@echo "Built $(STYLE)$(basename $@)$(NOSTYLE)"

bonus: $(LIBFT) $(BOBJ_FILES) $(BDEP_FILES) $(SRC_DIR)/$(NAME).h
	$(CC) $(INC) $(CFLAGS) $(LFLAGS) $(BOBJ_FILES) -o $(basename $@)
	@echo "Built $(STYLE)$(basename $@)$(NOSTYLE)"

$(LIBFT):
	make -C $(dir $@)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(@D)
	$(CC) $(INC) $(CFLAGS) -c $< -o $@

clean:
	$(RM) $(BUILD_DIR)/
	make fclean -C $(LIBFT_DIR)

fclean: clean
	$(RM) $(NAME)*
	$(RM) bonus*

re: fclean
	$(MAKE)

all: $(NAME)
