# Mentor_Graphics_test_task
For given task were designed two main abstractions <b>Parser</b> and <b>Rule</b>.<br>
The main idea is that every unit of filesystem can be processed by user defined parser 
with certain list of customizable rules.

<b>Parser</b> is an interface class which declaring following methods: <br>
<em>parse()</em> - the driver method does controlling execution of rules enforcement. <br>
<em>set_path()</em> <br>
<em>get_path()</em> <br>
<em>list_of_rules()</em> - returns reference to list container for rules management. <br>

<b>Rule</b> is an interface class which declaring following methods:<br>
<em>start_enforcement()</em> - implement processing of parsing unit. <br>
<em>is_errors()</em> -  returns true if errors ocures during enforcement.<br>
<em>get_info()</em> - returns info about errors. <br>

