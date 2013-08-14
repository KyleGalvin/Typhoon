all:	mastersthesis.tex
	latex mastersthesis.tex && \
	bibtex8 mastersthesis && \
	latex mastersthesis.tex && \
	dvipdfm mastersthesis.dvi && \
	latex2rtf -o ../mastersthesis.rtf ./mastersthesis.tex
	latex2html mastersthesis.tex -split 0 -no_navigation -info "" -address "" -html_version 4.0,unicode
	make -C mastersthesis
	rm -rf ../html
	cp -r ./mastersthesis/ ../html
