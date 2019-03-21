#!/bin/bash

htab1=( 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 )
htab2=( 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 )

drawDiagram() {
   
   # $1 - Pierwsza wspolrzedena poczatku wykresu
   # $2 - Druga wspolrzedna poczatku wykresu
   # $3 - Wysokosc slupka jaka ma zostac dodana do wykresu
   # $4 - Nazwa tablicy

   X=$1
   Y=$2
   H=$3

   przesuniecie() {
      for i in {0..18}
      do
	     eval "htab1[$i]=${htab1[$(($i + 1))]}"
      done
      eval "htab1[19]=$H"
   }
  
   drawColumn() {
      # $1 - Wysokosc do uzyskania
      # S2 - X w jakim ma rysowac
      # S3 - Y w jakim ma rysowac
      tput civis
      for i in $(seq 0 $(($1-1)))
      do
	  tput cup $(($3 - $i)) $2
	  tput setab 2
	  echo  ' '
	  tput sgr0
	  
      done
      tput cnorm

   }

   drawFrame() {
	tput civis
      	tput cup $(($Y + 13)) $X
	tput setab 7
	echo '                                          '
	tput sgr0

	tput cup $Y $X
	tput setab 7
        echo '                                          '
        tput sgr0

	tput cup $(($Y + 12)) $X
	for i in {1..12}
	do
		tput setab 7
		echo ' '
		tput sgr0
		tput cup $(($Y + $i)) $X

	done

	tput cup $(($Y + 12)) $(($X + 41))
	for i in {1..12}
        do
                tput setab 7
                echo ' '
                tput sgr0
		tput cup $(($Y + $i)) $(($X + 41))

        done
	tput cnorm
    }

    przesuniecie
    drawFrame
    
    startX=$(($X + 1))
    startY=$(($Y +12))
    tput cup $startY $startX
    for i in {1..19}
    do
	    
	    wys=${htab1[$i]}
	    drawColumn $wys $(($i * 2 + $startX)) $startY

    done
}

BYTES=$(cat /proc/net/dev | sed -n 3p | awk '{print $3}')
licznik=0
MEDIUM=0
while [ 1 ]
do
	
	BATERY_MAX=$(cat /sys/class/power_supply/BAT0/uevent | sed -n 9p | tr '=' ' ' | awk '{print $2}')	
	BATERY_ACT=$(cat /sys/class/power_supply/BAT0/uevent | sed -n 10p | tr '=' ' ' | awk '{print $2}')
	BATERY=$(($BATERY_ACT * 100 / $BATERY_MAX))
	TIME=$( cat /proc/uptime | awk '{print $1}' )
	TIME=$( echo "scale=0 ; $TIME * 100" | bc)
	TIME=${TIME%'.00'}
	TIME=$(( $TIME / 100))
	DNI=$(($TIME / 86400))
	TIME=$(($TIME - $(($DNI * 86400))))
	GODZ=$(($TIME / 3600))
	TIME=$(($TIME - $(($GODZ * 3600))))
	MIN=$(($TIME / 60))
	SEC=$(($TIME - $(($MIN * 60))))
	NEW_BYTES=$(cat /proc/net/dev | sed -n 3p | awk '{print $3}')
	SPEED=$((${NEW_BYTES} - $BYTES))
	MEDIUM=$(echo "scale =0 ; $(($MEDIUM * licznik + $SPEED)) / $(($licznik + 1 ))" | bc) 
	CPU=$( cat /proc/loadavg | awk '{print $1}')	
	BYTES=${NEW_BYTES}
	FLAGA=0
	if [ "$SPEED" -gt '1048576' ]
	then
		SPEED1=$(echo "scale =5 ; ${SPEED} / 1024 / 1024" | bc)
		FLAGA=2
	elif [ "$SPEED" -gt '1024' ]
	then
		SPEED1=$(echo "scale =5 ; ${SPEED} / 1024" | bc)
		FLAGA=1
	else
		SPEED1=$SPEED
	fi
	licznik=$(($licznik + 1))
	sleep 1
	clear
	
	echo "STAN BATERII: ${BATERY}%"
	echo "OBIAZENIE PROCESORA: $CPU"
	echo "CZAS PRACY MASZYNY: ${DNI}d ${GODZ}h ${MIN}min ${SEC}s"
	if [ $FLAGA == '0' ]
	then	
		echo "PREDKOSC INTERNETU: ${SPEED1}B"
	elif [ $FLAGA == '1' ]
	then
		echo "PREDKOSC INTERNETU: ${SPEED1}KB"
	else 
		echo "PREDKOSC INTERNETU: ${SPEED1}MB"
	fi

	if [ "$MEDIUM" -gt '1048576' ]
        then
		echo "SR PREDKOSC INTERNETU: $(echo "scale =5; ${MEDIUM} / 1024 / 1024" | bc)MB "
        elif [ "$MEDIUM" -gt '1024' ]
        then
                echo "SR PREDKOSC INTERNETU: $(echo "scale =5; ${MEDIUM} / 1024" | bc)KB "

	else
		echo "SR PREDKOSC INTERNETU: ${MEDIUM}B"
        fi
	
	high=$(echo "$CPU * 100 " | bc)
	high=${high%'.00'}
	high=$(( 11 * $high / 300 + 1 ))
	if [ $high -gt 11 ]
	then
		high=11
	fi
	tput cup 6 0
	echo "OBCIAZENIE SYSYTEMU 0.0 - 3.0 :"
	drawDiagram 0 7 ${high}

	
done



 

