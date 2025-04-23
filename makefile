CXX = clang++
CXXFLAGS = -std=c++20 -Iinclude
LDFLAGS = 

SRC_DIR = src
OBJS_DIR = obj
BIN = bin/VCS

SRCS = $(wildcard $(SRC_DIR)/*.cpp)
OBJS = $(SRCS:$(SRC_DIR)/%.cpp=$(OBJS_DIR)/%.o)

$(BIN): $(OBJS)
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) $^ -o $@ $(LDFLAGS)

$(OBJS_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJS) $(BIN)

rebuild: clean $(BIN)

.PHONY: clean rebuild
