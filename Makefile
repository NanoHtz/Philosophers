# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: fgalvez- <fgalvez-@student.42madrid.com    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/08/08 14:04:17 by fgalvez-          #+#    #+#              #
#    Updated: 2025/08/31 11:42:05 by fgalvez-         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# ========================= VARIABLES GLOBALES =============================== #

NAME         = philo
CC           = cc
CFLAGS       = -Wall -Wextra -Werror -g -O0

LIBS_FLAGS   = -L$(DIR_LIBFT) -lft

RM           = rm -f
NORMINETTE   = norminette
VALGRING     = valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes
VALGRING_OUT = valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --log-file=valgrind_output

# ========================= DIRECTORIOS Y ARCHIVOS =========================== #

DIR_HEADERS = Inc/Inc/libft
DIR_LIBFT   = Inc/libft/
DIR_PHILO   = Inc/
DIRSOURCE   = src/

HEADERS =	$(DIR_PHILO)philosophers.h \
			$(DIR_LIBFT)libft.h

SOURCES = $(DIRSOURCE)main.c \
			$(DIRSOURCE)inits.c \
			$(DIRSOURCE)routine.c \
			$(DIRSOURCE)utils.c \
			$(DIRSOURCE)take_forks.c \
			$(DIRSOURCE)sleep_think_eat.c \
			$(DIRSOURCE)control.c \
			$(DIRSOURCE)lock_unlock.c


# ========================= OBJETOS =========================== #

SRCS        = $(sort $(SOURCES))

OBJSDIR     = ./obj/
OBJS        = $(addprefix $(OBJSDIR), $(notdir $(SRCS:.c=.o)))

# ========================= COLORES PARA EL OUTPUT =========================== #

GREEN			= \033[0;32m
YELLOW			= \033[0;33m
CYAN			= \033[0;36m
MAGENTA			= \033[0;35m
RESET			= \033[0m
RED             = \033[0;31m

# ========================= REGLAS PRINCIPALES =============================== #
.PHONY: all clean fclean re n val ex err

all: libft $(NAME)

$(NAME): $(OBJS)
	@echo "\n${MAGENTA}Compilando el ejecutable $(NAME)...${RESET}\n"
	$(CC) $(OBJS) $(CFLAGS) $(LIBS_FLAGS) -o $(NAME)
	@echo "${CYAN}=================================================================================================================${RESET}"
	@echo "${GREEN}                                       [✔] $(NAME) successfully compiled.${RESET}                               "
	@echo "${CYAN}=================================================================================================================${RESET}"
	@echo "${MAGENTA}You should use: valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --log-file=valgrind_output ./$(NAME) ${RESET}"

# ========================= REGLAS PARA LOS OBJETOS ========================== #
$(OBJSDIR)%.o: $(DIRSOURCE)%.c
	@echo "${MAGENTA}Generando objetos de $(NAME)...${RESET}"
	@mkdir -p $(dir $@)
	@echo "${CYAN}Compilando objeto: $<${RESET}"
	$(CC) $(CFLAGS) $(addprefix -I, $(DIR_HEADERS)) -c $< -o $@

$(OBJSDIR)%.o: $(DIR_PHILO)/%.c
	@mkdir -p $(dir $@)
	@echo "${CYAN}Compilando objeto: $<${RESET}"
	$(CC) $(CFLAGS) $(addprefix -I, $(DIR_HEADERS)) -c $< -o $@

# ========================= LIMPIEZA DE ARCHIVOS ============================= #

libft:
	@$(MAKE) --no-print-directory -C $(DIR_LIBFT)

clean:
	@echo "${YELLOW}Limpiando archivos objeto de la $(NAME)...${RESET}"
	$(RM) -r $(OBJSDIR)
	@$(RM) valgrind_output
	@$(RM) errors.log
	@$(MAKE) --no-print-directory -C $(DIR_LIBFT) clean

fclean: clean
	@echo "${RED}Eliminando la biblioteca $(NAME)...${RESET}"
	$(RM) $(NAME)
	@$(MAKE) --no-print-directory -C $(DIR_LIBFT) fclean

re: fclean all
