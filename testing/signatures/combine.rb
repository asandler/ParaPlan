a = File.readlines(ARGV[0]).map{|line| line.to_f}
b = File.readlines(ARGV[0].gsub(".1", ".2")).map{|line| line.to_f}

a_mean = a.inject(:+) / a.size
b_mean = b.inject(:+) / b.size

a_top = a.sort[(a.size * 0.9).to_i..-1]
b_top = b.sort[(b.size * 0.9).to_i..-1]

a_top_mean = a_top.inject(:+) / a_top.size
b_top_mean = b_top.inject(:+) / b_top.size

name = ARGV[0]
if name.index('/')
    name = name[name.rindex('/') + 1..-3]
else
    name = name[0..-3]
end

#puts name + " " * (25 - name.size) + (b_mean / a_mean).to_s[0..8]
puts name + " " * (25 - name.size) + (b_top_mean / a_top_mean).to_s[0..8]
