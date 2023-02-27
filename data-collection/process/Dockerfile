# The base image to start from
FROM node:14-alpine3.12

# Setup a working directory for our app
WORKDIR /app

# Copy the application files
COPY . .

# Install the node modules
RUN npm install

# The final command that starts the app
CMD node ./process.js
