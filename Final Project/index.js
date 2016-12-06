 /**
  * vTrends is an Amazon Alexa app that reads headlines and news snippets from 
  * Google Trends.
  * 
  * This is a final student project for CS50, Harvard's Introduction to 
  * Computer Science class:
  * https://www.edx.org/course/introduction-computer-science-harvardx-cs50x
  * 
  * Many thanks to Harvard, edX and Professor David J. Malan for providing such
  * an outstanding learning experience for free.
  * 
  * Design patterns have been adapted from Amazon's TidePooler and SavvyShooper samples:
  * https://github.com/amzn/alexa-skills-kit-js/blob/master/samples/tidePooler
  * https://github.com/amzn/alexa-skills-kit-js/blob/master/samples/savvyConsumer
  * 
  * Speech objects are built using the ssml-builder library
  * https://www.npmjs.com/package/ssml-builder
  * 
  * Google Trends are fetched via the node API
  * https://www.npmjs.com/package/google-trends-api
  * 
  * MIT License
  * https://opensource.org/licenses/MIT
  * Copyright © 2016 Alex Erhardt
  */

/**
 * App ID for the skill
 */
var APP_ID = undefined;//replace with 'amzn1.echo-sdk-ams.app.[your-unique-value-here]';

/**
 * The AlexaSkill prototype and helper functions
 */
var AlexaSkill = require('./AlexaSkill');
var googleTrends = require('google-trends-api');
var S = require('string');
var Speech = require('ssml-builder');

/**
 * Session constants and variables
 */
var KEY_CURRENT_INDEX = "current";
var READ_LIMIT = 5;
var TOTAL_TREND_LIMIT = 20;
var ASSIGN_DEFAULT = true;

/**
 *  Supported countries and their codes
 */
var COUNTRIES = {
    'United States': 'UK',
    'United Kingdom': 'UK'
};

/**
 * Alternatives to how users might say the country names
 */
var SPOKEN_NAME_TO_COUNTRY_CODE = {
    'the united states': 'US',
    'united states': 'US',
    'the states': 'US',
    'the u s': 'US',
    'u s': 'US',
    'u s a': 'US',
    'united kingdom': 'UK',
    'the united kingdom': 'UK',
    'u k': 'UK',
    'great britain': 'UK'
};

/**
 * vTrends is a child of AlexaSkill
 */
var VTrends = function () {
    AlexaSkill.call(this, APP_ID);
};

// Extend Alexa Skill;
VTrends.prototype = Object.create(AlexaSkill.prototype);
VTrends.prototype.constructor = VTrends;

// Override AlexaSkill request and intent handlers
VTrends.prototype.eventHandlers.onSessionStarted = function (sessionStartedRequest, session) {
    console.log("vTrends onSessionStarted requestId: " + sessionStartedRequest.requestId
        + ", sessionId: " + session.sessionId);
};

VTrends.prototype.eventHandlers.onLaunch = function (launchRequest, session, response) {
    console.log("vTrends onLaunch requestId: " + launchRequest.requestId + ", sessionId: " + session.sessionId);
    handleWelcomeRequest(response);
};

VTrends.prototype.eventHandlers.onSessionEnded = function (sessionEndedRequest, session) {
    console.log("vTrends onSessionEnded requestId: " + sessionEndedRequest.requestId
        + ", sessionId: " + session.sessionId);
};

VTrends.prototype.intentHandlers = {
    "HotTrends": function(intent, session, response, inProgress) { 
        getTrends(intent, session, response, false);  
    },
    
    "HearMore": function(intent, session, response, inProgress) {
        getTrends(intent, session, response, true);
    },
    
    "DontHearMore": function(intent, session, response) {
        var speechOutput = "OK. Bye!";
        response.tell(speechOutput);
    },
    
    "ListCountries": function(intent, session, response) {
        handleSupportedCountriesRequest(intent, session, response);
    },
    
    "AMAZON.HelpIntent": function(intent, session, response) {
        handleHelpRequest(response);
    },
    
    "AMAZON.StopIntent": function(intent, session, response) {
        var speechOutput = "OK. Bye!";
        response.tell(speechOutput);
    },
    
    "AMAZON.CancelIntent": function(intent, session, response) {
        var speechOutput = "Goodbye";
        response.tell(speechOutput);
    }
};

/**
 * Handles reply when user opens the app
 */
function handleWelcomeRequest(response) {
    var whichCountryPrompt = "For which country would you like trends?",
        speechText = "Welcome to v Trends." + whichCountryPrompt,
        repromptText = "You need to tell me which country you want trends for. "
            + "Or you can simply open v Trends and ask a question like, "
            + "get trends for the United States. "
            + "For a list of supported countries, ask what countries are supported. "
            + whichCountryPrompt;
        
    response.ask(speechText, repromptText);
}

/**
 * Handles reply when user asks for help
 */
function handleHelpRequest(response) {
    var repromptText = "Which country would you like trends info for?",
        speechOutput = "You need to tell me which country you want trends for. "
        + "Or you can simply open v Trends and ask a question like, "
        + "get trends for the United States. "
        + "For a list of supported countries, ask what countries are supported. "
        + "Or you can say exit or stop at any time. "
        + "Which country would you like trends for?";
        + repromptText;
    
    response.ask(speechOutput, repromptText);
}

/**
 * Handles reply when user asks for list of available countries
 */
function handleSupportedCountriesRequest(intent, session, response) {
    var repromptText = "Which country would you like trends info for?",
        speechOutput = "I can retrieve trend information from these countries: "
            + getAllCountriesText()
            + repromptText;
    
    response.ask(speechOutput, repromptText);
}

/**
 * Gets Google Trends object and reads out first five items 
 */
function getTrends(intent, session, response, inProgress) {
    var speech = new Speech(),
        reprompt = new Speech(),
        speechOutput,
        repromptOutput,
        countryCode,
        currentTrendIndex;
    
    // Continuing to read a previously started trend    
    if(inProgress) {
        countryCode = session.attributes.countryCode;
        currentTrendIndex = session.attributes.currentTrendIndex;
    }
    // It's a fresh new trend
    else {
        countryCode = getCountryCodeFromIntent(intent, ASSIGN_DEFAULT);
        currentTrendIndex = 0;
    }
    
    // There was an error retrieving the country code
    if(countryCode.error) {
        speech.say("I'm not sure what the country is, please try again.");
        reprompt.say("I'm not sure what the country is, you can say, " +
        "for example, United States or U S." +
        "for a list of available countries, ask what countries are supported.");
        speechOutput = speech.toObject();
        repromptOutput = reprompt.toObject();
        response.ask(speechOutput, repromptOutput);
    }
    // Country code received a-OK
    else {
        // Store country code in session
        session.attributes.countryCode = countryCode;
        
        // Retrieve trend object; if successful, builds response string
        // Responses are built using ssml-builder
        googleTrends.hotTrendsDetail(countryCode)
        .then(function(trends){
        
            // Builds the reply with a max READ_LIMIT trends
            var currentReadLimit = currentTrendIndex + READ_LIMIT;
            for(currentTrendIndex; currentTrendIndex < currentReadLimit ; currentTrendIndex++) {
                var newsItem = getNewsItem(trends.rss.channel[0].item[currentTrendIndex]);
                var trendCounter = currentTrendIndex + 1;
                speech.say("Trend number " + trendCounter + " is: " + newsItem.keyword)
                .pause('1s').say(newsItem.headline).pause('1s').say(newsItem.snippet).pause('1s');
            }
            
            // If the trend is the last one, append goodbye message and end session 
            if(currentTrendIndex >= TOTAL_TREND_LIMIT) {
                speech.say("That was the last trend. Thanks for listening.");
                speechOutput = speech.toObject();
                response.tell(speechOutput);
            }
            // There are more trends to read, keep session open, ask user if they want more
            else {
                speech.say("Would you like to hear more?");
                reprompt.say("Would you like to hear more? Please say yay or nay.");
                speechOutput = speech.toObject();
                repromptOutput = reprompt.toObject;
                session.attributes.currentTrendIndex = currentTrendIndex;
                response.ask(speechOutput, repromptOutput);
            }
        })
        .catch(function(err){
            speech.say("I'm sorry, I cannot get the trends for " 
            + countryCode.country + " at this" +
            " time. Please try again later. Goodbye.");
            speechOutput = speech.toObject();
            response.tell(speechOutput);
            console.error(err.stack);
        });
    }
}

/**
 * Returns the country code from the intent, or returns an error
 */
function getCountryCodeFromIntent(intent, assignDefault) {
    
    var countrySlot = intent.slots.Country;
    
    // Test for empty slot, or empty value
    if(!countrySlot || !countrySlot.value) {
        if(!assignDefault) {
            return {
                error: true
            };
        }
        else {
            return {
                country: 'United States',
                countryCode: 'US'
            };
        }
    }
    else {
        var countryName = countrySlot.value;
        if(SPOKEN_NAME_TO_COUNTRY_CODE[countryName.toLowerCase()]) {
            var countryCode = SPOKEN_NAME_TO_COUNTRY_CODE[countryName.toLowerCase()];
            return countryCode;
        }
        else {
            return {
                error: true,
                country: countryName
            };
        }
    }
    
} 

/**
 * Provides list of supported countries
 */
function getAllCountriesText() {
    var countryList = '';
    
    for (var country in COUNTRIES) {
        countryList += country + ", ";
    }
    
    return countryList;
}

/**
 * Chooses which news item to read from a trend, deciding based on headline
 * completeness and length. Returns an object with readable strings (ie, no
 * HTML tags, encoding or incomplete sentences).
 */
function getNewsItem(trend) {
    
    // object constructor containing all the info that Alexa will read
    function NewsItem (headline, snippet, medium, keyword) {
        this.headline = headline;
        this.snippet = snippet;
        this.medium = medium;
        this.keyword = keyword;
    }
    
    // object constructor for new headline
    function Headline(headline, index, full) {
        this.headline = headline;
        this.index = index;
        this.full = full;
    }
    
    // empty array to store headlines and properties
    var headlineArr = [];
    
    // news items from trend object (normally 1 or 2)
    var newsItems = trend["ht:news_item"];

    // iterates through news items, cleans up headlines, and stores whether they are complete or not
    var fullCount = 0;
    for(var i = 0; i < newsItems.length; i++) {
        var cleanHeadline = S(newsItems[i]['ht:news_item_title'][0]).decodeHTMLEntities().stripTags().s; 
        var index = i;
        var full = false;
        
        // if headline doesn't end in "..." mark as full
        if( (cleanHeadline.endsWith('...')) === false) {
            full = true;
            fullCount++;
        }
        
        // create object and push into array
        var newHeadline = new Headline(cleanHeadline, index, full);
        headlineArr.push(newHeadline);
    }

    // returns a final object depending on headline completeness and length
    // case 1: there are no complete headlines, simply return the longest ones
    var longest, filtered, finalHeadline, finalIndex;
    if(fullCount === 0) {
        longest = longestHeadline(headlineArr);
        finalHeadline = longest.headline;
        finalIndex = longest.index;
    }
    // case 2: there is exactly one complete headline, choose that one
    else if(fullCount === 1) {
        filtered = headlineArr.filter(filterByComplete);
        finalHeadline = filtered[0].headline;
        finalIndex = filtered[0].index;
    }
    // case 3: there are two or more complete headlines; pick the longest one
    else if(fullCount >= 2) {
        filtered = headlineArr.filter(filterByComplete);
        longest = longestHeadline(filtered);
        finalHeadline = longest.headline;
        finalIndex = longest.index;
    }
    
    // Cleans up the snippet, and if it ends with an incomplete sentence, removes it
    var cleanSnippet = S(newsItems[finalIndex]["ht:news_item_snippet"][0]).decodeHTMLEntities().stripTags().s;
    if(cleanSnippet.endsWith('...')) {
        cleanSnippet = snippetCutter(cleanSnippet);
    }
    var finalSnippet = cleanSnippet;
    var finalMedium = newsItems[finalIndex]["ht:news_item_source"][0];
    var keyword = trend["title"];
    
    var finalItem = new NewsItem(finalHeadline, finalSnippet, finalMedium, keyword);
    
    return finalItem;
}

/**
 * Filters complete headlines (not ending in '...')
 */
function filterByComplete(headline) {
    if(headline.full === true) {
        return true;
    }
    else {
        return false;
    }
}

/**
 * Returns the longest headline from two different ones
 */
function longestHeadline(arr) {
    var l = 0;
    for (var i = 0; i < arr.length; i++) {
        if (arr[l].headline.length < arr[i].headline.length) l = i;
    }
    return arr[l];
}

/**
 * Cuts a news snippet if it ends in an incomplete sentence, leaving only the
 * last complete sentence.
 */
function snippetCutter(snippet) {
    // Checks for the position of the last "..." marking incomplete last sentence
    var endIndex = snippet.lastIndexOf('...') - 1;
    
    // Checks for possible positions of sentence before last
    var exclamationPos = snippet.lastIndexOf('!', endIndex);
    var questionPos = snippet.lastIndexOf('?', endIndex);
    var periodPos = snippet.lastIndexOf('.', endIndex);
    
    // Cuts the snippet at the last known position of complete sentence
    var cutIndex = Math.max(periodPos, exclamationPos, questionPos);
    var cutSnippet = snippet.substring(0, cutIndex + 1);
    
    return cutSnippet;
}

// Handler that responds to Alexa Request
exports.handler = function(event, context) {
    var vTrends = new VTrends();
    vTrends.execute(event, context);
};