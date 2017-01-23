make

FQ=test.cnt.gz
rm -f ${FQ}{,.gbi}

lines=500000
python tests/make-test-fastq.py $lines | bgzip -c > $FQ
echo "indexing"
time ./grabix index $FQ
echo "indexed"
python tests/test-fastq.py $FQ
a=$(./grabix grab test.cnt.gz $(($lines * 4)))
b=$(zless $FQ | tail -1)
if [[ "$a" != "$b" ]]; then
	echo FAIL last record
fi
rm -f ${FQ}{,.gbi}

for V in  \
	test.PLs.vcf \
	test.auto_dom.no_parents.2.vcf \
	test.auto_dom.no_parents.3.vcf \
	test.auto_dom.no_parents.4.vcf \
	test.auto_dom.no_parents.5.vcf \
	test.auto_dom.no_parents.vcf \
	test.auto_dom.vcf \
	test.auto_rec.no_parents.2.vcf \
	test.auto_rec.no_parents.3.vcf \
	test.auto_rec.no_parents.4.vcf \
	test.auto_rec.no_parents.5.vcf \
	test.auto_rec.no_parents.vcf \
	test.auto_rec.vcf \
	test.burden.vcf \
	test.cadd.vcf \
	test.clinvar.vcf \
	test.comp_het.2.vcf \
	test.comp_het.3.vcf \
	test.comp_het.4.vcf \
	test.comp_het.5.vcf \
	test.comp_het.6.vcf \
	test.comp_het.7.vcf \
	test.comp_het.vcf \
	test.cosmic.vcf \
	test.de_novo.affected.and.unaffected.vcf \
	test.de_novo.vcf \
	test.esp.zero.vcf \
	test.exac.vcf \
	test.family.vcf \
	test.fusions.vcf \
	test.mendel.vcf \
	test.multiple-alts.decomp.snpeff.vcf \
	test.passonly.vcf \
	test.query.vcf \
	test.region.vep.vcf \
	test.roh.vcf \
	test.snpeff.vcf \
	test.somatic.vcf \
	test.vcf_id.snpeff.vcf \
	test1.snpeff.vcf \
	test2.snpeff.vcf \
	test3.snpeff.vcf \
	test4.vep.snpeff.vcf \
	test5.vep.snpeff.vcf; do

	rm -f ${V}.*
	wget --quiet https://raw.githubusercontent.com/arq5x/gemini/master/test/$V
	bgzip -f $V
	./grabix index ${V}.gz
	sleep 1
	exp=$(zgrep -cv "#" $V.gz)
	obs=$(./grabix size $V.gz)

	if [[ "$exp" != "$obs" ]]; then
		echo "FAIL: $V: expected $exp lines found $obs"
	else 
		echo "OK $V"
	fi
	rm -f ${V}.*

done
