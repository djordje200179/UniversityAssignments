package com.djordjemilanovic.minesweeper
package utils

import utils.Move.Button

case class Move(button: Button, location: Location) {
  override def toString: String = s"${button.toString}(${location.x + 1},${location.y + 1})"
}

object Move {
  def tryParse(str: String): Option[Move] = {
    val pattern = raw"(\w)\((\d+),(\d+)\)".r
    str match {
      case pattern(buttonChar, xStr, yStr) => for {
        button <- Button.values.find(_.toString.head == buttonChar.head)
        x <- xStr.toIntOption
        y <- yStr.toIntOption
      } yield Move(button, Location(y - 1, x - 1))
      case _ => None
    }
  }

  enum Button(char: Char) {
    case Left extends Button('L')
    case Right extends Button('R')

    override def toString: String = char.toString
  }
}
