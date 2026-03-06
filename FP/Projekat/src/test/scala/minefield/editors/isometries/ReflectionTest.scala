package com.djordjemilanovic.minesweeper
package minefield.editors.isometries

import minefield.Minefield.Cell.*
import minefield.editors.Isometry
import minefield.editors.Transforming.ReflectionAxis.*
import minefield.sampleMinefield
import utils.{Location, Sector}

import org.scalatest.funsuite.AnyFunSuite

class ReflectionTest extends AnyFunSuite {
  test("Horizontal reflection around origin") {
    val sector = Sector(Location(1, 0), Location(2, 2))
    val isometry = Reflection(sector.dimensions, Horizontal, Some(0))

    val result = Isometry(false, true)(isometry)(sector)(sampleMinefield)
    assert(result(Location(0, 1)) == sampleMinefield(Location(2, 1)))
  }

  test("Main diagonal reflection") {
    val sector = Sector(Location(1, 1), Location(2, 2))
    val isometry = Reflection(sector.dimensions, MainDiagonal, None)

    val result = Isometry(false, true)(isometry)(sector)(sampleMinefield)
    assert(result(Location(1, 2)) == Empty)
    assert(result(Location(2, 1)) == Mine)
  }

  test("Anti diagonal reflection") {
    val sector = Sector(Location(1, 0), Location(2, 1))
    val isometry = Reflection(sector.dimensions, AntiDiagonal, None)

    val result = Isometry(false, true)(isometry)(sector)(sampleMinefield)
    assert(result(Location(1, 0)) == Empty)
    assert(result(Location(2, 1)) == Mine)
  }

  test("Central vertical reflection") {
    val isometry = Reflection(sampleMinefield.dimensions, Vertical, Some(1))

    val result = Isometry(false, true)(isometry)(Sector(Location(0, 0), Location(2, 2)))(sampleMinefield)
    assert(result == sampleMinefield)
  }
}
