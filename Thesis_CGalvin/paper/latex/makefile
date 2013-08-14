all:	paper	bundle

paper:	mastersthesis.tex
	latex mastersthesis.tex
	bibtex8 mastersthesis
	latex mastersthesis.tex
	dvipdfm mastersthesis.dvi
	latex2rtf -o mastersthesis.rtf mastersthesis.tex	
	latex2html mastersthesis.tex -split 0 -no_navigation -info "" -address "" -html_version 4.0,unicode
	rm -rf ../html
	sudo libreoffice --headless --convert-to odt:"OpenDocument Text Flat XML" ./mastersthesis/index.html
	sudo libreoffice --headless --convert-to doc ./mastersthesis/index.html
	mv ./mastersthesis/ ../html

bundle:
	rm -rf Kyle_Galvin_Project 
	mkdir Kyle_Galvin_Project
	cp ./index.doc Kyle_Galvin_Project/Kyle_Galvin_Thesis.doc
	cp ./index.odt Kyle_Galvin_Project/Kyle_Galvin_Thesis.odt
	cp ./mastersthesis.pdf Kyle_Galvin_Project/Kyle_Galvin_Thesis.pdf
	cp ./mastersthesis.rtf Kyle_Galvin_Project/Kyle_Galvin_Thesis.rtf

clean:
	rm -rf ../html
	rm -rf Kyle_Galvin_Project
	rm -f ./index.doc	
	rm -f ./index.odt
	rm -f ./texput.log
	rm -f ./mastersthesis.toc
	rm -f ./mastersthesis.pdf
	rm -f ./mastersthesis.dvi
	rm -f ./mastersthesis.blg
	rm -f ./mastersthesis.log
	rm -f ./mastersthesis.aux
	rm -f ./mastersthesis.bbl
