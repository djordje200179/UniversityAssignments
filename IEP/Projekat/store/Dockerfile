FROM python:3

RUN mkdir /code
WORKDIR /code

COPY requirements.txt requirements.txt
RUN pip install -r requirements.txt

COPY *.py .
COPY contracts ./contracts

ARG START_FILE
ENV START_FILE=$START_FILE

ENTRYPOINT python $START_FILE