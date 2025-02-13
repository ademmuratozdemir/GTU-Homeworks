JC = javac
JFLAGS = -classpath .
JD = javadoc
JDFLAGS = -protected -splitindex -use -author -version -d ./javadoc
RM = rm
JR = java

CLASSES = \
		FileSystemElement.java \
		File.java \
		Driver.java \
		Directory.java \
		FileSystem.java \
		
all : Driver.class

run : 
	$(JR) Driver

classes : $(CLASSES:.java=.class)

%.class : %.java
	$(JC) $(JFLAGS) $<

doc:
	$(JD) $(JDFLAGS) *.java 

clean:
	$(RM) *.class 

cleandoc:
	$(RM) -r ./javadoc