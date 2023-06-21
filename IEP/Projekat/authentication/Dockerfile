FROM python:3

RUN mkdir /code
WORKDIR /code

COPY requirements.txt requirements.txt
COPY *.py .

RUN pip install -r requirements.txt

ENTRYPOINT ["python", "app.py"]