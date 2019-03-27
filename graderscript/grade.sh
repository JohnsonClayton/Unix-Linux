rm $1
rm studoutput$2.txt
if g++ $1.cpp -o $1 2> errors.txt; then
	
	if ./$1 < input$2.txt > studoutput$2.txt ; then
		if	diff -y output$2.txt studoutput$2.txt > differences.txt ; then
			echo "It works!"
		else
			cat differences.txt
		fi
	else
		echo "You should return 0"
	fi
else
	cat errors.txt
fi
