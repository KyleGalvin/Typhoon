all:	mastersthesis.tex
	latex mastersthesis.tex && \
	bibtex8 mastersthesis && \
	latex mastersthesis.tex && \
	dvipdfm mastersthesis.dvi && \
	latex2rtf -o ../mastersthesis.rtf ./mastersthesis.tex
	latex2html mastersthesis.tex
	rm -rf ../html
	mv ./mastersthesis ../html
