all: dvi ps pdf html

dvi:
	latex thesis-ex.tex
	bibtex thesis-ex.aux
	latex thesis-ex.tex
	latex thesis-ex.tex

ps: dvi
	dvips -o thesis-ex.ps thesis-ex.dvi

pdf: dvi
	dvipdf thesis-ex.dvi thesis-ex.pdf

html:
	mkdir -p thesis-ex
	cp uiucthesis.sty thesis-ex
	cp jeffe.sty thesis-ex
	latex2html -dir thesis-ex -local_icons -html_version 3.2,math -info 0 -address 'Matthew Hayward <a href="http://github.com/digitalmacgyver/quant/tree/master/thesis/">Lower Query Bounds in the Quantum Oracle Model GitHub Repository</a>' -bottom_navigation thesis-ex.tex

clean:
	rm -rf thesis-ex/
	rm -f thesis-ex.dvi
	rm -f thesis-ex.aux


