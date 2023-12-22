<?php
const PUZZLE_PATH = "puzzle.txt";

function part1()
{
	$f = fopen(PUZZLE_PATH, "r") or die("[ERROR] Unable to open file.");
	$total = 0;
	while(!feof($f)) {
		$line = explode(":", fgets($f));
		if(count($line) > 1) {
			$cardNumber = $line[0];
			$content    = $line[1];
		} else break;
		$numbers	= explode(" | ", $content);
		$winningNumbers = array_filter(explode(" ", $numbers[0]));
		$yourNumbers	= array_filter(explode(" ", $numbers[1]));
		$cardScore      = 0;
		foreach($winningNumbers as $winning) {
			foreach($yourNumbers as $yours) {
				if((int) $yours == (int) $winning) {
					$cardScore++;
					break;
				}
			}
		}
		if($cardScore > 0) $total += 2**($cardScore-1);
	}

	echo "Part 1: " . $total . "\n";
	fclose($f);
}

function getWinningCards($f, $cardNumber)
{
	static $cardDictionary = array();
	if(isset($cardDictionary[$cardNumber])) {
		$winningCards = $cardDictionary[$cardNumber];
		return $winningCards;
	}

	$winningCards = array();
	fseek($f, 0);
	for($i = 0; $i < $cardNumber-1; ++$i) fgets($f);
	$line = explode(":", fgets($f));
	if(count($line) > 1) $content = $line[1];
	$numbers    	= explode(" | ", $content);
	$winningNumbers = array_filter(explode(" ", $numbers[0]));
	$yourNumbers    = array_filter(explode(" ", $numbers[1]));

	$wonCard = $cardNumber;
	foreach($winningNumbers as $winning) {
		foreach($yourNumbers as $yours) {
			if((int) $yours == (int) $winning) {
				array_push($winningCards, ++$wonCard);
			}
		}
	}

	$cardDictionary[$cardNumber] = $winningCards;
	return $winningCards;
}

function part2()
{
	$f = fopen(PUZZLE_PATH, "r") or die("[ERROR] Unable to open file.");
	$totalNumberOfCards = count(file(PUZZLE_PATH));
	$total = $totalNumberOfCards;
	for($i = 0; $i < $totalNumberOfCards; ++$i) {
		echo "processing " . $i . "\n";
		$winningCards = getWinningCards($f, $i+1);
		while(count($winningCards) > 0) {
			$numberOfCurrentCards = count($winningCards);
			foreach($winningCards as $card) {
				$total++;
				$winningCards =
					array_merge($winningCards, getWinningCards($f, $card));
			}
			array_splice($winningCards, 0, $numberOfCurrentCards);
			echo $numberOfCurrentCards . "\n";
		}
	}

	echo "Part 2: " . $total . "\n";
	fclose($f);
}

part1();
part2();

?>
