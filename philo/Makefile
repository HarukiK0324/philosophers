CC = cc
CFLAGS = -Wall -Wextra -Werror -I.
NAME = philo

SRC_DIR = .
OBJ_DIR = obj

SRC = $(SRC_DIR)/init.c $(SRC_DIR)/main.c $(SRC_DIR)/philo_utils.c $(SRC_DIR)/threads.c $(SRC_DIR)/utils.c
OBJ = $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SRC))

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) $(OBJ) -lpthread -o $(NAME)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

clean:
	rm -rf $(OBJ_DIR)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re