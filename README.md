grabix - a wee tool for random access into BGZF files.
======================================================

``grabix`` leverages the fantastic BGZF library in ``samtools`` to provide random access into
text files that have been compressed with ``bgzip``.  ``grabix`` creates it's own index (.gbi)
of the bgzipped file.  Once index, one can extract arbitrary lines from the file with the ``grab`` command.

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