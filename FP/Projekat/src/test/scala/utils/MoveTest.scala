package com.djordjemilanovic.minesweeper
package utils

import utils.Move.Button

import org.scalatest.funsuite.AnyFunSuite

class MoveTest extends AnyFunSuite {
  test("Parsing empty string") {
    assert(Move.tryParse("").isEmpty)
  }

  test("Parsing single character string") {
    assert(Move.tryParse("a").isEmpty)
  }

  test("Parsing bracketed string") {
    assert(Move.tryParse("(1,2)").isEmpty)
  }

  test("Parsing valid string") {
    assert(Move.tryParse("L(3,4)").contains(Move(Button.Left, Location(3, 2))))
  }

  test("Parsing invalid button") {
    assert(Move.tryParse("X(3,4)").isEmpty)
  }

  test("String representation") {
    val move = Move(Button.Right, Location(5, 6))
    assert(move.toString == "R(7,6)")
  }
}