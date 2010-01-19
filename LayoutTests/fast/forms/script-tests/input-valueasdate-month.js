description('Tests for .valueAsDate with &lt;input type=month>.');

var input = document.createElement('input');
input.type = 'month';

function valueAsDateFor(stringValue) {
    input.value = stringValue;
    return input.valueAsDate;
}

function setValueAsDateAndGetValue(year, month, day) {
    var date = new Date();
    date.setTime(Date.UTC(year, month, day));
    input.valueAsDate = date;
    return input.value;
}

shouldBe('valueAsDateFor("")', 'null');
shouldBe('valueAsDateFor("1969-12").getTime()', 'Date.UTC(1969, 11, 1, 0, 0, 0, 0)');
shouldBe('valueAsDateFor("1970-01").getTime()', 'Date.UTC(1970, 0, 1)');
shouldBe('valueAsDateFor("2009-12").getTime()', 'Date.UTC(2009, 11, 1)');

shouldBe('setValueAsDateAndGetValue(1969, 11, 1)', '"1969-12"');
shouldBe('setValueAsDateAndGetValue(1970, 0, 1)', '"1970-01"');
shouldBe('setValueAsDateAndGetValue(2009, 11, 31)', '"2009-12"');
shouldBe('setValueAsDateAndGetValue(10000, 0, 1)', '"10000-01"');

shouldBe('setValueAsDateAndGetValue(794, 9, 22)', '""');
shouldBe('setValueAsDateAndGetValue(1582, 8, 30)', '""');
shouldBe('setValueAsDateAndGetValue(1582, 9, 1)', '"1582-10"');
shouldBe('setValueAsDateAndGetValue(1582, 9, 31)', '"1582-10"');
shouldBe('setValueAsDateAndGetValue(275760, 8, 13)', '"275760-09"');
shouldBe('setValueAsDateAndGetValue(275760, 8, 14)', '""'); // Date of JavaScript can't represent this.

debug('Sets null to valueAsDate:');
input.value = '2009-12';
input.valueAsDate = null;
shouldBe('input.value', '""');

var successfullyParsed = true;
