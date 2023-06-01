# Use the python:3.8-slim-buster image as a base
FROM python:3.8-slim-buster

# Set the working directory to /app
WORKDIR /app

# Copy the current directory contents into the container at /app
COPY requirements.txt requirements.txt

# Install any needed packages specified in requirements.txt
RUN pip3 install -r requirements.txt

COPY . .

# Run gobbler.py when the container launches
CMD ["python3", "-u", "mqtt-gobbler.py"]