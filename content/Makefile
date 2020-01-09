all: build_lib build_samples
clean: clean_lib clean_samples

build_lib: XML_Manager.o
	ar rvs ./XML_Manager/XML_Manager.a ./XML_Manager/XML_Manager.o
	rm ./XML_Manager/*.o
	
XML_Manager.o:
	g++ -c ./XML_Manager/XML_Manager.cpp
	mv XML_Manager.o ./XML_Manager/XML_Manager.o
	
build_samples: build_lib
	g++ -o ./Examples/Ex_01/Sample_01 ./Examples/Ex_01/Main_01.cpp ./XML_Manager/XML_Manager.a
	g++ -o ./Examples/Ex_02/Sample_02 ./Examples/Ex_02/Main_02.cpp ./XML_Manager/XML_Manager.a
	g++ -o ./Examples/Ex_03/Sample_03 ./Examples/Ex_03/Main_03.cpp ./XML_Manager/XML_Manager.a
	
clean_lib:
	rm ./XML_Manager/XML_Manager.a -rf

clean_samples:
	rm ./Examples/Ex_01/Sample_01 -rf
	rm ./Examples/Ex_01/Sample_02 -rf
	rm ./Examples/Ex_01/Sample_03 -rf