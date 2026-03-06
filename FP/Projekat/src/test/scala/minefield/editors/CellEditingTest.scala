package com.djordjemilanovic.minesweeper
package minefield.editors

import minefield.Minefield.Cell.Empty
import minefield.editors.CellEditing.*
import minefield.sampleMinefield
import utils.{Location, Sector}

import org.scalatest.funsuite.AnyFunSuite

class CellEditingTest extends AnyFunSuite {
  test("Updating cell") {
    val location = Location(1, 2)

    val newMinefield = updatedCell(Empty, location)(sampleMinefield)

    assert(newMinefield(location) == Empty)
  }

  test("Clearing area") {
    val newMinefield = clearedSector(Sector(Location(1, 1), Location(2, 2)))(sampleMinefield)

    assert(newMinefield.minesCount == 2)
  }

  test("Invalid area clearing") {
    assertThrows[IllegalArgumentException] {
      clearedSector(Sector(Location(2,2), Location(1,1)))(sampleMinefield)
    }
  }
}