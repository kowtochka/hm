# Компилятор и флаги
CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -pedantic -g
CAIRO_CFLAGS = $(shell pkg-config --cflags cairo x11)
CAIRO_LIBS = $(shell pkg-config --libs cairo x11)
LDFLAGS = -lm

# Директории
SRC_DIR = src
EXAMPLES_DIR = examples
BUILD_DIR = build
LIB_DIR = $(BUILD_DIR)/lib
BIN_DIR = $(BUILD_DIR)/bin

## Директории для установки
INSTALL_PREFIX = /usr/local
INSTALL_LIB_DIR = $(INSTALL_PREFIX)/lib
INSTALL_INCLUDE_DIR = $(INSTALL_PREFIX)/include
INSTALL_BIN_DIR = $(INSTALL_PREFIX)/bin

# Библиотека
LIB_NAME = graphics
LIB_STATIC = $(LIB_DIR)/lib$(LIB_NAME).a
LIB_SHARED = $(LIB_DIR)/lib$(LIB_NAME).so
LIB_HEADER = $(SRC_DIR)/graphics.h
LIB_VERSION = 1.0.0


# Примеры
EXAMPLES_SRC = $(wildcard $(EXAMPLES_DIR)/*.c)
EXAMPLES_BIN = $(patsubst $(EXAMPLES_DIR)/%.c,$(BIN_DIR)/%,$(EXAMPLES_SRC))

# Основная цель по умолчанию
all: library install install-shared setup-bashrc examples

# Проверка зависимостей
check-deps:
	@pkg-config --exists cairo x11 || { \
		echo "Ошибка: Не найдены необходимые библиотеки cairo и/или X11"; \
		echo "Установите их с помощью пакетного менеджера:"; \
		echo "  Ubuntu/Debian: sudo apt-get install libcairo2-dev libx11-dev"; \
		echo "  Fedora/RHEL: sudo dnf install cairo-devel libX11-devel"; \
		echo "  Arch: sudo pacman -S cairo libx11"; \
		exit 1; \
	}

# Создание директорий
$(BUILD_DIR) $(LIB_DIR) $(BIN_DIR):
	@mkdir -p $@

# Компиляция библиотеки
library: check-deps $(LIB_STATIC)

$(LIB_STATIC): $(SRC_DIR)/graphics.c $(SRC_DIR)/graphics.h | $(LIB_DIR)
	$(CC) $(CFLAGS) $(CAIRO_CFLAGS) -c $< -o $(LIB_DIR)/graphics.o
	ar rcs $@ $(LIB_DIR)/graphics.o

shared: check-deps $(LIB_SHARED)

$(LIB_SHARED): $(SRC_DIR)/graphics.c $(SRC_DIR)/graphics.h | $(LIB_DIR)
	$(CC) $(CFLAGS) -fPIC $(CAIRO_CFLAGS) -c $< -o $(LIB_DIR)/graphics.o
	$(CC) -shared -Wl,-soname,lib$(LIB_NAME).so.1 -o $@.$(LIB_VERSION) $(LIB_DIR)/graphics.o $(CAIRO_LIBS) $(LDFLAGS)
	ln -sf lib$(LIB_NAME).so.$(LIB_VERSION) $(LIB_DIR)/lib$(LIB_NAME).so
	ln -sf lib$(LIB_NAME).so.$(LIB_VERSION) $(LIB_DIR)/lib$(LIB_NAME).so.1


# ==================== УСТАНОВКА БИБЛИОТЕКИ ====================

# Установка библиотеки в систему
install: $(LIB_STATIC) $(LIB_HEADER)
	@echo "Установка библиотеки $(LIB_NAME) в $(INSTALL_PREFIX)"
	@mkdir -p $(INSTALL_LIB_DIR) $(INSTALL_INCLUDE_DIR)
	cp $(LIB_STATIC) $(INSTALL_LIB_DIR)/
	cp $(LIB_HEADER) $(INSTALL_INCLUDE_DIR)/
	@echo "Библиотека установлена успешно"

# Установка динамической библиотеки
install-shared: shared
	@echo "Установка динамической библиотеки $(LIB_NAME) в $(INSTALL_PREFIX)"
	@mkdir -p $(INSTALL_LIB_DIR) $(INSTALL_INCLUDE_DIR)
	cp $(LIB_SHARED).$(LIB_VERSION) $(INSTALL_LIB_DIR)/
	cp $(LIB_HEADER) $(INSTALL_INCLUDE_DIR)/
	cd $(INSTALL_LIB_DIR) && \
		ln -sf lib$(LIB_NAME).so.$(LIB_VERSION) lib$(LIB_NAME).so && \
		ln -sf lib$(LIB_NAME).so.$(LIB_VERSION) lib$(LIB_NAME).so.1
	ldconfig
	@echo "Динамическая библиотека установлена успешно"

setup-bashrc:
	@echo "Добавление LD_LIBRARY_PATH в .bashrc..."
	@if ! grep -q "LD_LIBRARY_PATH.*/usr/local/lib" ~/.bashrc; then \
		echo 'export LD_LIBRARY_PATH=$$LD_LIBRARY_PATH:/usr/local/lib' >> ~/.bashrc; \
		echo "Добавлено в .bashrc"; \
	else \
		echo "Путь уже существует в .bashrc"; \
	fi

setup-bashrc-stud:
	@echo "Добавление LD_LIBRARY_PATH в .bashrc..."
	@if ! grep -q "LD_LIBRARY_PATH.*/usr/local/lib" /home/stud/.bashrc; then \
		echo 'export LD_LIBRARY_PATH=$$LD_LIBRARY_PATH:/usr/local/lib' >> /home/stud/.bashrc; \
		echo "Добавлено в .bashrc"; \
	else \
		echo "Путь уже существует в .bashrc"; \
	fi


# Компиляция примеров
examples: $(EXAMPLES_BIN)

$(BIN_DIR)/%: $(EXAMPLES_DIR)/%.c $(LIB_STATIC) | $(BIN_DIR)
	$(CC) $(CFLAGS) $(CAIRO_CFLAGS) -I$(SRC_DIR) $< -L$(LIB_DIR) -l$(LIB_NAME) $(CAIRO_LIBS) $(LDFLAGS) -o $@

# Запуск конкретного примера
run-%: $(BIN_DIR)/%
	@echo "Запуск примера: $*"
	@cd $(BIN_DIR) && ./$*

# Отладочная сборка
debug: CFLAGS += -DDEBUG -O0
debug: clean all

# Релизная сборка
release: CFLAGS += -O2 -DNDEBUG
release: clean all

# Полезные цели
clean:
	rm -rf $(BUILD_DIR)

distclean: clean
	rm -f *~

list-examples:
	@echo "Доступные примеры:"
	@for example in $(notdir $(EXAMPLES_BIN)); do \
		echo "  - $$example"; \
	done

info:
	@echo "=== Информация о проекте ==="
	@echo "Библиотека: $(LIB_STATIC)"
	@echo "Примеры: $(notdir $(EXAMPLES_BIN))"
	@echo "Флаги компиляции: $(CFLAGS)"
	@echo "Флаги cairo: $(CAIRO_CFLAGS)"
	@echo "Библиотеки cairo: $(CAIRO_LIBS)"

install-deps-ubuntu:
	sudo apt-get update
	sudo apt-get install libcairo2-dev libx11-dev pkg-config

install-deps-fedora:
	sudo dnf install cairo-devel libX11-devel pkg-config

install-deps-arch:
	sudo pacman -S cairo libx11 pkg-config

help:
	@echo "Доступные цели:"
	@echo "  all               - собрать библиотеку и все примеры"
	@echo "  library           - собрать только библиотеку"
	@echo "  examples          - собрать только примеры"
	@echo "  run-<name>        - запустить конкретный пример (например: run-demo-shapes)"
	@echo "  debug             - собрать с отладочной информацией"
	@echo "  release           - собрать с оптимизацией"
	@echo "  list-examples     - показать список всех примеров"
	@echo "  info              - показать информацию о сборке"
	@echo "  install-deps-*    - установить зависимости (ubuntu/fedora/arch)"
	@echo "  clean             - удалить собранные файлы"
	@echo "  distclean         - полная очистка"
	@echo "  help              - показать эту справку"

# Список всех исполняемых файлов примеров
.PHONY: all library examples clean distclean list-examples help run-% check-deps debug release info install-deps-ubuntu install-deps-fedora install-deps-arch

# Зависимости для примеров
$(BIN_DIR)/demo-shapes: $(LIB_STATIC)
$(BIN_DIR)/demo-animated-shapes: $(LIB_STATIC)
$(BIN_DIR)/demo-animation-menu: $(LIB_STATIC)
$(BIN_DIR)/demo-custom-keyboard-control: $(LIB_STATIC)
$(BIN_DIR)/demo-particle-system: $(LIB_STATIC)
$(BIN_DIR)/demo-sine-animation: $(LIB_STATIC)
$(BIN_DIR)/demo-text: $(LIB_STATIC)
$(BIN_DIR)/demo-animated-text: $(LIB_STATIC)
