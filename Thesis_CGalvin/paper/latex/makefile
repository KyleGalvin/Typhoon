all:	paper	bundle

paper:	mastersthesis.tex
	latex mastersthesis.tex
	bibtex8 mastersthesis
	latex mastersthesis.tex
	dvipdfm mastersthesis.dvi
	latex2rtf -o ../mastersth`esis.rtf ./mastersthesis.tex	
	latex2html mastersthesis.tex -split 0 -no_navigation -info "" -address "" -html_version 4.0,unicode
	rm -rf ../html
	mv -r ./mastersthesis/ ../html
	sudo libreoffice --headless --convert-to odt:"OpenDocument Text Flat XML" ./mastersthesis/index.html
	sudo libreoffice --headless --convert-to doc ./mastersthesis/index.html

bundle:
	rm -rf Kyle_Galvin_Project 
	mkdir Kyle_Galvin_Project
	cp ./index.doc Kyle_Galvin_Project
	cp ./index.odt Kyle_Galvin_Project
	cp ./mastersthesis.pdf Kyle_Galvin_Project

clean:
	rm -rf ../html
	rm -rf Kyle_Galvin_Project
	rm ./index.doc	
	rm ./index.odt
