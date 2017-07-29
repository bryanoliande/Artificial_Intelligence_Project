rm -f SmartAI;
rm -f AverageAI;
rm -f PoorAI;
rm -f RandomAI;
rm -f MyAI;

if [ $(uname) == Linux ]; then
	# echo "Running SmartAI";
	# for filename in ../Worlds/*.txt; do ../SmartAI $filename SmartAI; done
	echo "Running AverageAI";
	for filename in ../Worlds/*.txt; do ../AverageAI $filename AverageAI; done
	echo "Running PoorAI";
	for filename in ../Worlds/*.txt; do ../PoorAI $filename PoorAI; done
	echo "Running RandomAI";
	for filename in ../Worlds/*.txt; do ../RandomAI $filename RandomAI; done
	echo "Running MyAI";
	for filename in ../Worlds/*.txt; do ../../bin/MyAI $filename MyAI; done

	echo "";
	echo "===============================SCORES===============================";
	./Scorer SmartAI AverageAI PoorAI RandomAI MyAI;
	echo "====================================================================";
	echo "";
else
	# echo "Running SmartAI";
	# for filename in ../Worlds/*.txt; do ../SmartAI.exe $filename SmartAI; done
	echo "Running AverageAI";
	for filename in ../Worlds/*.txt; do ../AverageAI.exe $filename AverageAI; done
	echo "Running PoorAI";
	for filename in ../Worlds/*.txt; do ../PoorAI.exe $filename PoorAI; done
	echo "Running RandomAI";
	for filename in ../Worlds/*.txt; do ../RandomAI.exe $filename RandomAI; done
	echo "Running MyAI";
	for filename in ../Worlds/*.txt; do ../../bin/MyAI.exe $filename MyAI; done
	
	echo ""
	echo "===============================SCORES==============================="
	./Scorer.exe SmartAI AverageAI PoorAI RandomAI MyAI;
	echo "===================================================================="
	echo ""
fi

rm -f SmartAI;
rm -f AverageAI;
rm -f PoorAI;
rm -f RandomAI;
rm -f MyAI;