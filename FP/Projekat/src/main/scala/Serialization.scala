package com.djordjemilanovic.minesweeper

import minefield.Minefield
import utils.Move

import java.io.*
import scala.io.Source
import scala.util.{Try, Using}

object Serialization {
  def readMinefield(file: File): Try[Minefield] = Using(Source.fromFile(file)) (_.getLines().map(_.toVector).toVector).flatMap(Minefield.parse)
  def writeMinefield(minefield: Minefield)(file: File): Try[Unit] = Using(new FileWriter(file)) (_.write(minefield.toString))

  def readBoard(file: File): Try[Board] = Using(new ObjectInputStream(new FileInputStream(file))) (_.readObject().asInstanceOf[Board])
  def writeBoard(board: Board)(file: File): Try[Unit] = Using(new ObjectOutputStream(new FileOutputStream(file))) (_.writeObject(board))

  private val resultsFile = new File("results.mres")

  def readResults: Try[Results] = Using(new ObjectInputStream(new FileInputStream(resultsFile))) (_.readObject().asInstanceOf[Results])
  def writeResults(results: Results): Try[Unit] = Using(new ObjectOutputStream(new FileOutputStream(resultsFile))) (_.writeObject(results))
  
  def readActions(file: File): Try[Seq[Move]] = Using(Source.fromFile(file))(_.getLines().flatMap(Move.tryParse).toSeq)
}
