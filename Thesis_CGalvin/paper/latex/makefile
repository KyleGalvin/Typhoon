all:	mastersthesis.tex
	pdflatex mastersthesis.tex && \
	bibtex8 mastersthesis && \
	pdflatex mastersthesis.tex && \
	dvipdfm mastersthesis.dvi && \
	latex2rtf -o ../mastersthesis.rtf ./mastersthesis.tex
