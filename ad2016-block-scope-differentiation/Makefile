DOC:=slides

simple:
	@pdflatex $(DOC).tex

all:
	@pdflatex $(DOC).tex
	@pdflatex $(DOC).tex
	@bibtex $(DOC).aux
	@pdflatex $(DOC).tex

.PHONY: clean
clean:
	@rm -f *.aux *.toc *.log *.blg *.bbl *.nav *.out *.snm *.vrb *~

.PHONY: distclean
distclean:
	$(MAKE) clean
	@rm -f $(DOC).pdf
