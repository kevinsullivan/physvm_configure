import .PeirceOutput
#check "FLAG"
#check "CHECK"
#check launch_time_in_seconds
#check "EVAL"
#eval launch_time_in_seconds
#check "FLAG"
#check "CHECK"
#check [mk_time std.value 4.000000]
#check "EVAL"
#eval [mk_time std.value 4.000000]
#check "FLAG"
#check "CHECK"
#check launch_time_in_years
#check "EVAL"
#eval launch_time_in_years
#check "FLAG"
#check "CHECK"
#check [mk_time der.value 4.000000]
#check "EVAL"
#eval [mk_time der.value 4.000000]
#check "FLAG"
#check "CHECK"
#check one_second
#check "EVAL"
#eval one_second
#check "FLAG"
#check "CHECK"
#check [mk_duration std.value 4.000000]
#check "EVAL"
#eval [mk_duration std.value 4.000000]
#check "FLAG"
#check "CHECK"
#check one_year
#check "EVAL"
#eval one_year
#check "FLAG"
#check "CHECK"
#check _
#check "EVAL"
#eval _
#check "FLAG"
#check "CHECK"
#check two_seconds
#check "EVAL"
#eval two_seconds
#check "FLAG"
#check "CHECK"
#check one_second+ᵥone_second
#check "EVAL"
#eval one_second+ᵥone_second
#check "FLAG"
#check "CHECK"
#check one_second_after_launch
#check "EVAL"
#eval one_second_after_launch
#check "FLAG"
#check "CHECK"
#check launch_time_in_seconds+ᵥone_second
#check "EVAL"
#eval launch_time_in_seconds+ᵥone_second
#check "FLAG"
#check "CHECK"
#check what_after_what
#check "EVAL"
#eval what_after_what
#check "FLAG"
#check "CHECK"
#check launch_time_in_years+ᵥone_second
#check "EVAL"
#eval launch_time_in_years+ᵥone_second
#check "FLAG"
#check "CHECK"
#check two_years
#check "EVAL"
#eval two_years
#check "FLAG"
#check "CHECK"
#check one_second+ᵥone_second
#check "EVAL"
#eval one_second+ᵥone_second
#check "FLAG"
