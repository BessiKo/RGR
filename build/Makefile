# Компилятор и флаги
CXX := g++
CXXFLAGS := -std=c++17 -Wall -Wextra -fPIC -I../lib -I../src
LDFLAGS := -shared
LDLIBS := -ldl

# Директории
SRC_DIR := ../src
LIB_SRC_DIR := ../lib
OBJ_DIR := obj
BIN_DIR := .
LIB_DIR := .

# Исходные файлы
MAIN_SRC := $(SRC_DIR)/main.cpp
SWITCH_SRC := $(SRC_DIR)/switch/switch.cpp
FILE_SRC := $(SRC_DIR)/file/file.cpp
GENERAL_SRC := $(SRC_DIR)/general/general.cpp
MENU_UTILS_SRC := $(SRC_DIR)/menu_utils/menu_utils.cpp
PASSWD_SRC := $(SRC_DIR)/passwd/passwd.cpp

GAMMA_SRC := $(LIB_SRC_DIR)/gamma/gamma.cpp
MATRIX_SRC := $(LIB_SRC_DIR)/matrix/matrix.cpp
PERMUTATION_SRC := $(LIB_SRC_DIR)/permutation/permutation.cpp

# Объектные файлы
MAIN_OBJ := $(OBJ_DIR)/main.o
SWITCH_OBJ := $(OBJ_DIR)/switch.o
FILE_OBJ := $(OBJ_DIR)/file.o
GENERAL_OBJ := $(OBJ_DIR)/general.o
MENU_UTILS_OBJ := $(OBJ_DIR)/menu_utils.o
PASSWD_OBJ := $(OBJ_DIR)/passwd.o

GAMMA_OBJ := $(OBJ_DIR)/gamma.o
MATRIX_OBJ := $(OBJ_DIR)/matrix.o
PERMUTATION_OBJ := $(OBJ_DIR)/permutation.o

# Целевые файлы
TARGET := ciphers
LIBRARIES := libgamma.so libmatrix.so libpermutation.so

.PHONY: all clean install

all: directories $(LIBRARIES) $(TARGET)

directories:
	@mkdir -p $(OBJ_DIR)

# Основное приложение
$(TARGET): $(MAIN_OBJ) $(SWITCH_OBJ) $(FILE_OBJ) $(GENERAL_OBJ) $(MENU_UTILS_OBJ) $(PASSWD_OBJ)
	$(CXX) $(CXXFLAGS) $^ -o $(BIN_DIR)/$@ $(LDLIBS)

# Библиотеки
libgamma.so: $(GAMMA_OBJ)
	$(CXX) $(LDFLAGS) $^ -o $(LIB_DIR)/$@

libmatrix.so: $(MATRIX_OBJ)
	$(CXX) $(LDFLAGS) $^ -o $(LIB_DIR)/$@

libpermutation.so: $(PERMUTATION_OBJ)
	$(CXX) $(LDFLAGS) $^ -o $(LIB_DIR)/$@

# Компиляция объектных файлов
$(MAIN_OBJ): $(MAIN_SRC)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(SWITCH_OBJ): $(SWITCH_SRC) $(SRC_DIR)/switch/switch.h
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(FILE_OBJ): $(FILE_SRC) $(SRC_DIR)/file/file.h
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(GENERAL_OBJ): $(GENERAL_SRC) $(SRC_DIR)/general/general.h
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(MENU_UTILS_OBJ): $(MENU_UTILS_SRC) $(SRC_DIR)/menu_utils/menu_utils.h
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(PASSWD_OBJ): $(PASSWD_SRC) $(SRC_DIR)/passwd/passwd.h
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(GAMMA_OBJ): $(GAMMA_SRC) $(LIB_SRC_DIR)/gamma/gamma.h
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(MATRIX_OBJ): $(MATRIX_SRC) $(LIB_SRC_DIR)/matrix/matrix.h
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(PERMUTATION_OBJ): $(PERMUTATION_SRC) $(LIB_SRC_DIR)/permutation/permutation.h
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Установка
install: all
	@sudo cp $(LIBRARIES) /usr/local/lib/
	@sudo ldconfig
	@sudo cp $(TARGET) /usr/local/bin/
	@echo "Установка завершена"

# Очистка
clean:
	@rm -rf $(OBJ_DIR) $(LIBRARIES) $(TARGET)
	@echo "Очистка завершена"