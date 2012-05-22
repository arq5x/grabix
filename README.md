grabix - a wee tool for random access into BGZF files.
======================================================

``grabix`` leverages the fantastic BGZF library in ``samtools`` to provide random access into
text files that have been compressed with ``bgzip``.  ``grabix`` creates it's own index (.gbi)
of the bgzipped file.  Once indexed, one can extract arbitrary lines from the file with the ``grab`` command.  
Or choose random lines with the, well, ``random`` command.

There's a ton of room for improvement, but I needed something quickly in support of a side project.

Here's a brief example using the ``simrep.chr1.bed`` file provided in the repository.

	# 1. compress the file with bgzip
	bgzip simrep.chr1.bed
	
	# 2. create a grabix index of the file.
	#    creates simrep.chr1.bed.gbi
	grabix index simrep.chr1.bed.gz
	
	# 3. now, extract the 100th line in the file.
	grabix grab simrep.chr1.bed.gz 100
	chr1	401285	401444	trf	218
	
	# 4. extract the 100th through 110th lines in the file.
	grabix grab simrep.chr1.bed.gz 100 110
	chr1	401285	401444	trf	218
	chr1	401573	401748	trf	280
	chr1	404661	404707	trf	92
	chr1	406202	406274	trf	76
	chr1	406227	406286	trf	77
	chr1	406776	406819	trf	68
	chr1	409821	409866	trf	51
	chr1	409865	409900	trf	52
	chr1	421245	421285	trf	64
	chr1	422395	422435	trf	80
	chr1	422560	422588	trf	56
	
You can also use ``grabix`` to extract random lines from the file

	# extract 10 randome lines from the file using reservoir sampling
	grabix random simrep.chr1.bed.gz 10