
# issues of embedding fonts in pdf doc. The figures created using MS word has some problem to embed fonts when printing 
# them using ps printer driver. finally, we are using Linux openoffice to print it to ps file and use ps2epsi and ps2pdf utilities to 
# create eps and pdf figure file.  
# The process is as follows:
# 1. print the figure in (open office) to a file and save it as a *.ps file
# 2. ps2epsi command to create the *.epsi file and rename it to *.eps file
# 3.  ps2pdf -dEmbedAllFonts=true -dSubsetFonts=true -dEPSCrop=true *.eps to create cropped pdf file
# Now both the latex and pdflatex works 

LATEX=latex
PDFLATEX=pdflatex
BIBTEX=bibtex

SRCPRE=omp

SRC=$(SRCPRE).tex
BIBFILE=$(SRCPRE)
PDF=$(SRC:.tex=.pdf)

default: pdf

dvi:
	make clean
	$(LATEX) $(SRC)
	-$(BIBTEX) $(BIBFILE)
	$(LATEX) $(SRC)
	$(LATEX) $(SRC)

pdf:
	$(PDFLATEX) $(SRC)
	-$(BIBTEX) $(BIBFILE)
	$(PDFLATEX) $(SRC)
	$(PDFLATEX) $(SRC)

dvipdf: dvi
	dvips -Ppdf -G0 -tletter ${SRCPRE}
	ps2pdf -dCompatibilityLevel=1.4 -dMAxSubsetPct=100 -dSubsetFonts=true -dEmbedAllFonts=true -sPAPERSIZE=letter ${SRCPRE}.ps
#	$(PDFLATEX) $(SRC)
#	-$(BIBTEX) $(BIBFILE)
#	$(PDFLATEX) $(SRC)
#	$(PDFLATEX) $(SRC)

view: dvi
#	xdvi $(SRCPRE).dvi	
	evince $(SRCPRE).dvi	 >/dev/null 2>&1&

viewpdf: pdf
	acroread $(SRCPRE).pdf >/dev/null 2>&1&

clean:
	rm -f *.log *.aux *.dvi *.ps  *.bbl *.aux *.blg *.toc

clobber:clean
	rm -rf *.bak
