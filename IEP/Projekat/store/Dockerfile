FROM python:3

RUN mkdir /code
WORKDIR /code

COPY requirements.txt requirements.txt
COPY *.py .

RUN pip install -r requirements.txt

ARG START_FILE
ENV START_FILE=$START_FILE

ENTRYPOINT python $START_FILE