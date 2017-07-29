RAW_SOURCES = \
	Agent.hpp\
	main.cpp\
	Tile.hpp\
	World.cpp\
	World.hpp\
	MyAI.cpp\
	MyAI.hpp

SOURCE_DIR = src
BIN_DIR = bin
DOC_DIR = doc
SOURCES = $(foreach s, $(RAW_SOURCES), $(SOURCE_DIR)/$(s))

.PHONY: all clean score

all: $(SOURCES)
	g++ -std=c++0x $(SOURCES) -o $(BIN_DIR)/MyAI

submission: all
	rm -f submission.zip
	zip -r submission.zip $(SOURCE_DIR) $(BIN_DIR) $(DOC_DIR)

clean:
	rm -f submission.zip
	rm -f $(BIN_DIR)/*

score:
	$(MAKE) -C _Resources/ScorerScripts all