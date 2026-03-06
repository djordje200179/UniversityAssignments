package com.djordjemilanovic.minesweeper
package minefield

enum Difficulty(val maxArea: Int, val maxMines: Int) {
  case Easy extends Difficulty(50, 10)
  case Medium extends Difficulty(170, 20)
  case Hard extends Difficulty(900, 100)
  case Brutal extends Difficulty(Int.MaxValue, Int.MaxValue)
}