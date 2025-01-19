CXX = @c++

CXXFLAGS = -Wall -Wextra -Werror -fsanitize=address #-std=c++98
LDFLAGS = -fsanitize=address

SRCS = main.cpp srcs/Client.cpp srcs/Server.cpp srcs/Command.cpp  srcs/Channel.cpp srcs/Utils.cpp
OBJS = $(SRCS:.cpp=.o)

EXEC = IRC

all: $(EXEC)
	@echo "\033[1;32mIRC SERVER AND CLIENT HAS BEEN COMPILED SUCCESSFULLY!\033[0m"
	@echo "\033[1;32mUSAGE: ./IRC [PORT] [PASSWD]\033[0m"
	@echo "\033[1;32mYOUR IP ADDRESS IS:\033[0m"
	@ipconfig getifaddr en0

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(EXEC): $(OBJS)
	$(CXX) $(OBJS) -o $(EXEC) $(LDFLAGS)

clean:
	@rm -fr $(OBJS)

fclean: clean
	@rm -fr $(EXEC)

re: fclean all