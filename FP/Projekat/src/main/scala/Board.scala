package com.djordjemilanovic.minesweeper

import minefield.Minefield
import minefield.Minefield.Cell.Mine
import utils.{Dimensions, Location}

import scala.util.{Failure, Try}

class Board private(private val minefield: Minefield, private val field: Vector[Vector[Board.Cell]]) extends Serializable {
  val leftMarks: Int = minefield.minesCount - field.flatten.count(Board.Cell.Marked.eq)
  val isCleared: Boolean = field.flatten.count {
    case Board.Cell.Closed | Board.Cell.Marked => true
    case _ => false
  } == minefield.minesCount

  def dimensions: Dimensions = minefield.dimensions

  def apply(location: Location): Board.Cell = field(location.y)(location.x)

  def hint: Location = (
    for
      y <- 0 until dimensions.height
      x <- 0 until dimensions.width
      location = Location(y, x)
      if field(y)(x) == Board.Cell.Closed && minefield(location) != Mine
    yield location
    ).head

  private def updated(newField: Vector[Vector[Board.Cell]]) = new Board(minefield, newField)

  def markedCell(location: Location): Board = {
    val currentCell = field(location.y)(location.x)
    val updatedCell = currentCell match {
      case Board.Cell.Closed => Board.Cell.Marked
      case Board.Cell.Marked => Board.Cell.Closed
      case _ => currentCell
    }

    updated(field.updated(location.y, field(location.y).updated(location.x, updatedCell)))
  }

  def openedCell(location: Location): Try[Board] = {
    if (minefield(location) == Minefield.Cell.Mine) {
      return Failure(Board.MineOpened(location))
    }

    def openedCellRecursive(newField: Array[Array[Board.Cell]])(location: Location): Unit = {
      minefield(location) match {
        case Minefield.Cell.Empty if minefield.nearbyMines(location) == 0 =>
          newField(location.y)(location.x) = Board.Cell.Opened(0)

          for {
            location <- location.neighbors(dimensions)
            if newField(location.y)(location.x) == Board.Cell.Closed
          } openedCellRecursive(newField)(location)
        case Minefield.Cell.Empty =>
          newField(location.y)(location.x) = Board.Cell.Opened(minefield.nearbyMines(location))
        case Mine => assert(false, "Mine shouldn't be encountered during expansion")
      }
    }

    Try {
      val newField = field.map(_.toArray).toArray
      openedCellRecursive(newField)(location)
      updated(newField.map(_.toVector).toVector)
    }
  }

  override def toString: String = field.map(_.map(_.toString).mkString).mkString(System.lineSeparator)
}

object Board {
  enum Cell(representation: Char) {
    case Closed extends Cell('■')
    case Marked extends Cell('⚑')
    case Opened(neighbouringMines: Int) extends Cell(if neighbouringMines > 0 then ('0' + neighbouringMines).toChar else ' ')

    override def toString: String = representation.toString
  }

  def apply(minefield: Minefield) = new Board(
    minefield,
    Vector.fill(minefield.dimensions.height, minefield.dimensions.width)(Cell.Closed)
  )

  final case class MineOpened(location: Location) extends Exception(s"There was a mine at $location!")
}