#Variables
NAME		= ircserv
CXX			= c++
CXXFLAGS	= -Wall -Wextra -Werror -I.
CPPFLAGS	= -std=c++98
RM			= rm -rf
SRCS		= $(wildcard *.cpp)
OBJS		= $(SRCS:.cpp=.o)

#Colors
DEF_COLOR	= \033[0;39m
GRAY		= \033[0;90m
RED			= \033[0;91m
GREEN		= \033[0;92m
YELLOW		= \033[0;93m
BLUE		= \033[0;94m
MAGENTA		= \033[0;95m
CYAN		= \033[0;96m
WHITE		= \033[0;97m

#Arguments for run
ifeq (run,$(firstword $(MAKECMDGOALS)))
  RUN_ARGS := $(wordlist 2,$(words $(MAKECMDGOALS)),$(MAKECMDGOALS))
  $(eval $(RUN_ARGS):;@:)
endif

all:		$(NAME)
run:		all
			@echo "$(GREEN)runing prog $(NAME).$(DEF_COLOR)"
			./$(NAME) $(RUN_ARGS)

$(NAME):	$(OBJS)
			$(CXX) $(CXXFLAGS) $(CPPFLAGS) $(OBJS) -o $(NAME)
			@echo "$(GREEN)$(NAME) compiled!$(DEF_COLOR)"

clean:
			@$(RM) $(OBJS)
			@echo "$(YELLOW)Cleaned!$(DEF_COLOR)"

fclean: clean
			@$(RM) $(NAME)
			@echo "$(YELLOW)All cleaned!$(DEF_COLOR)"

re: fclean all

server: all
			./$(NAME) 667 123
client:
			nc localhost 667

.PHONY:		all clean fclean re run server client