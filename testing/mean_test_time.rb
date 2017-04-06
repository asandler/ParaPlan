a = []
count = 0

if not ARGV[0] or not ARGV[1]
    puts "Arguments needed: lower bound for first item, upper bound for last item"
    exit 1
end

while s = $stdin.gets
    next if s.split[0].to_f < ARGV[0].to_f
    next if s.split[-1].to_f > ARGV[1].to_f

    if a.empty?
        a = s.split.map{|s| s.to_f}
    else
        s.split.map{|s| s.to_f}.each_with_index{|v, i| a[i] += v}
    end

    count += 1
end

a.map!{|v| (v /= count).to_s[0..4] }

puts a.join(' ')
puts a.map{|v| (a[0].to_f / v.to_f).to_s[0..4]}.join(' ')
