package com.djordjemilanovic.minesweeper

import Results.Result
import utils.Dimensions

class Results private(results: List[Result]) extends Serializable with Iterable[Result] {
  override def iterator: Iterator[Result] = results.iterator

  def inserted(newResult: Result): Results = {
    val (before, after) = results.span(r => r.score > newResult.score)
    new Results(before ++ (newResult :: after).take(Results.keepAtMost) )
  }
}

object Results {
  val empty: Results = new Results(Nil)
  private val keepAtMost: Int = 10

  case class Result(dimensions: Dimensions, score: Int)
}