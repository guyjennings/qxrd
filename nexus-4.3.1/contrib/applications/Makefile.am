## Process this file with automake to produce Makefile.in
#====================================================================
#  
# $Id: Makefile.am 1283 2009-10-14 17:50:43Z Freddie Akeroyd $
#
# @configure_input@
#
#====================================================================
#if HAVE_LIBXML2
#NXTRANSLATE=NXtranslate
#endif
#SUBDIRS = NXdir $(NXTRANSLATE)
if HAVE_CBFLIB
CBFLIB=CBFLib
endif
#if HAVE_LIBMXMLPLUS
NXEXTRACT=NXextract
#endif
SUBDIRS=$(CBFLIB) $(NXEXTRACT)
