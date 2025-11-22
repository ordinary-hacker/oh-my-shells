CC = gcc
CFLAGS = -std=c11
TEST_CFLAGS = -std=c11 -DUNITY_INCLUDE_DOUBLE
TARGET = oh-my-shells
SRCDIR = src
LIBDIR = lib/tomlc17/src
UNITY_DIR = lib/unity/src
TEST_DIR = tests
SOURCES = $(wildcard $(SRCDIR)/*.c) $(LIBDIR)/tomlc17.c
TEST_SOURCES = $(filter-out $(SRCDIR)/main.c, $(SOURCES))
UNITY_SOURCES = $(UNITY_DIR)/unity.c

$(TARGET): $(SOURCES)
	$(CC) $(SOURCES) -o $(TARGET) $(CFLAGS)

test_cli: $(TEST_DIR)/test_cli.c $(TEST_SOURCES) $(UNITY_SOURCES)
	$(CC) $(TEST_DIR)/test_cli.c $(TEST_SOURCES) $(UNITY_SOURCES) -o $(TEST_DIR)/test_cli $(TEST_CFLAGS)

test_module_loader: $(TEST_DIR)/test_module_loader.c $(TEST_SOURCES) $(UNITY_SOURCES)
	$(CC) $(TEST_DIR)/test_module_loader.c $(TEST_SOURCES) $(UNITY_SOURCES) -o $(TEST_DIR)/test_module_loader $(TEST_CFLAGS)

test_integration: $(TEST_DIR)/test_integration.c $(TEST_SOURCES) $(UNITY_SOURCES)
	$(CC) $(TEST_DIR)/test_integration.c $(TEST_SOURCES) $(UNITY_SOURCES) -o $(TEST_DIR)/test_integration $(TEST_CFLAGS)

test_blackbox: $(TEST_DIR)/test_blackbox.c $(UNITY_SOURCES)
	$(CC) $(TEST_DIR)/test_blackbox.c $(UNITY_SOURCES) -o $(TEST_DIR)/test_blackbox $(TEST_CFLAGS)

build-tests: test_cli test_module_loader test_integration test_blackbox

run-test-cli: test_cli
	./$(TEST_DIR)/test_cli

run-test-module-loader: test_module_loader
	./$(TEST_DIR)/test_module_loader

run-test-integration: test_integration
	./$(TEST_DIR)/test_integration

run-test-blackbox: $(TARGET) test_blackbox
	./$(TEST_DIR)/test_blackbox

test: $(TARGET) build-tests
	@echo "Running CLI tests..."
	@./$(TEST_DIR)/test_cli
	@echo "\nRunning Module Loader tests..."
	@./$(TEST_DIR)/test_module_loader
	@echo "\nRunning Integration tests..."
	@./$(TEST_DIR)/test_integration
	@echo "\nRunning Black Box tests..."
	@./$(TEST_DIR)/test_blackbox
	@echo "\nAll tests completed!"

clean:
	rm -f $(TARGET)
	rm -f $(TEST_DIR)/test_cli $(TEST_DIR)/test_module_loader $(TEST_DIR)/test_integration $(TEST_DIR)/test_blackbox

.PHONY: clean test build-tests run-test-cli run-test-module-loader run-test-integration run-test-blackbox
