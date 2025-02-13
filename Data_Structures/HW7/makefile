JC = javac
JFLAGS = -g
TARGET = Main
SOURCES = Main.java AVLTree.java GUIVisualization.java Stock.java StockDataManager.java
DOCS = doc

default: $(TARGET)

$(TARGET): $(SOURCES)
	$(JC) $(JFLAGS) $^
	java -Xint $(TARGET) random1.txt

doc:
	javadoc -d $(DOCS) $(SOURCES)

clean:
	rm -f *.class

clean-doc:
	rm -rf $(DOCS)

.PHONY: clean default doc clean-doc
