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

part1();

?>
