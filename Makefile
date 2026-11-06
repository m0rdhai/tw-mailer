CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -pthread

CLIENT_TARGET = twmailer-client
SERVER_TARGET = twmailer-server

CLIENT_SRCS = main.cpp ClientSocket.cpp ClientUI.cpp ProtocolSerializerValidator.cpp
SERVER_SRCS = main_server.cpp Server.cpp ProtocolSerializerValidator.cpp

CLIENT_OBJS = $(CLIENT_SRCS:.cpp=.o)
SERVER_OBJS = $(SERVER_SRCS:.cpp=.o)

all: $(CLIENT_TARGET) $(SERVER_TARGET)

$(CLIENT_TARGET): $(CLIENT_OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

$(SERVER_TARGET): $(SERVER_OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(CLIENT_OBJS) $(SERVER_OBJS) $(CLIENT_TARGET) $(SERVER_TARGET)

.PHONY: all clean