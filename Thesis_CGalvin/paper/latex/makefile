all:	paper	format	bundle

paper:	mastersthesis.tex
	latex mastersthesis.tex
	bibtex8 mastersthesis
	latex mastersthesis.tex
	latex mastersthesis.tex
	dvipdfm mastersthesis.dvi
	latex2rtf -o mastersthesis.rtf mastersthesis.tex	
	latex2html mastersthesis.tex -split 0 -no_navigation -info "" -address "" -html_version 4.0,unicode -debug
	rm -rf ../html

format: odt	doc
	
odt:	./mastersthesis/mastersthesis.html
	unoconv -f -o mastersthesis.odt ./mastersthesis/mastersthesis.html
#	sudo libreoffice --headless --convert-to odt:"OpenDocument Text Flat XML" ./mastersthesis/mastersthesis.html

doc:	./mastersthesis.odt
	unoconv -f docx-o mastersthesis.docx ./mastersthesis/mastersthesis.html
	unoconv -f doc -o mastersthesis.doc ./mastersthesis/mastersthesis.html
	#libreoffice --headless --convert-to docx ./mastersthesis.odt

bundle:
	mv ./mastersthesis/ ../html
	rm -rf Kyle_Galvin_Project 
	mkdir Kyle_Galvin_Project
	cp ./mastersthesis.doc Kyle_Galvin_Project/Kyle_Galvin_Thesis.doc
	cp ./mastersthesis.odt Kyle_Galvin_Project/Kyle_Galvin_Thesis.odt
	cp ./mastersthesis.pdf Kyle_Galvin_Project/Kyle_Galvin_Thesis.pdf
	cp ./mastersthesis.pdf ../../../webDev/node/final/js/public/Kyle_Galvin_Thesis.pdf
	cp ./mastersthesis.rtf Kyle_Galvin_Project/Kyle_Galvin_Thesis.rtf

clean:
	rm -rf ../html
	rm -rf Kyle_Galvin_Project
	rm -f ./index.doc	
	rm -f ./index.odt
	rm -f ./texput.log
	rm -f ./mastersthesis.toc
	rm -f ./mastersthesis.doc
	rm -f ./mastersthesis.docx
	rm -f ./mastersthesis.pdf
	rm -f ./mastersthesis.dvi
	rm -f ./mastersthesis.blg
	rm -f ./mastersthesis.log
	rm -f ./mastersthesis.aux
	rm -f ./mastersthesis.bbl
	rm -f ./mastersthesis.rtf
	rm -f ./mastersthesis.out
	rm -f ./mastersthesis.odt
	rm -rf ./mastersthesis
