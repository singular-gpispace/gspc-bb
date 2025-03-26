#pragma once

//#define DEBUG_BBA

#define USE_KNF true


#define NO_NAME_MANGLING extern "C"

#include <string>
#include <config.hpp>
#include <boost/variant.hpp>

#include <iostream>

#include <we/type/literal/control.hpp>
#include <we/type/bitsetofint.hpp>
#include <we/type/bytearray.hpp>
#include <we/type/value.hpp>
#include <map>

#define CONTROL_TOKEN we::type::literal::control{}

#define READ_GENERATORS_STATICALLY_WITH_PERMUTATION(generators, r, permutation) \
for(int k=r; k>1000; k--) {generators.emplace_front(RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, k, permutation));} \
switch (std::min(r,1000)) \
{ \
  case 1000:  static poly static_ideal_generator_1000 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 1000, permutation);  generators.emplace_front(static_ideal_generator_1000); \
	case 999:  static poly static_ideal_generator_999 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 999, permutation);  generators.emplace_front(static_ideal_generator_999); \
	case 998:  static poly static_ideal_generator_998 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 998, permutation);  generators.emplace_front(static_ideal_generator_998); \
	case 997:  static poly static_ideal_generator_997 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 997, permutation);  generators.emplace_front(static_ideal_generator_997); \
	case 996:  static poly static_ideal_generator_996 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 996, permutation);  generators.emplace_front(static_ideal_generator_996); \
	case 995:  static poly static_ideal_generator_995 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 995, permutation);  generators.emplace_front(static_ideal_generator_995); \
	case 994:  static poly static_ideal_generator_994 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 994, permutation);  generators.emplace_front(static_ideal_generator_994); \
	case 993:  static poly static_ideal_generator_993 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 993, permutation);  generators.emplace_front(static_ideal_generator_993); \
	case 992:  static poly static_ideal_generator_992 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 992, permutation);  generators.emplace_front(static_ideal_generator_992); \
	case 991:  static poly static_ideal_generator_991 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 991, permutation);  generators.emplace_front(static_ideal_generator_991); \
	case 990:  static poly static_ideal_generator_990 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 990, permutation);  generators.emplace_front(static_ideal_generator_990); \
	case 989:  static poly static_ideal_generator_989 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 989, permutation);  generators.emplace_front(static_ideal_generator_989); \
	case 988:  static poly static_ideal_generator_988 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 988, permutation);  generators.emplace_front(static_ideal_generator_988); \
	case 987:  static poly static_ideal_generator_987 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 987, permutation);  generators.emplace_front(static_ideal_generator_987); \
	case 986:  static poly static_ideal_generator_986 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 986, permutation);  generators.emplace_front(static_ideal_generator_986); \
	case 985:  static poly static_ideal_generator_985 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 985, permutation);  generators.emplace_front(static_ideal_generator_985); \
	case 984:  static poly static_ideal_generator_984 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 984, permutation);  generators.emplace_front(static_ideal_generator_984); \
	case 983:  static poly static_ideal_generator_983 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 983, permutation);  generators.emplace_front(static_ideal_generator_983); \
	case 982:  static poly static_ideal_generator_982 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 982, permutation);  generators.emplace_front(static_ideal_generator_982); \
	case 981:  static poly static_ideal_generator_981 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 981, permutation);  generators.emplace_front(static_ideal_generator_981); \
	case 980:  static poly static_ideal_generator_980 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 980, permutation);  generators.emplace_front(static_ideal_generator_980); \
	case 979:  static poly static_ideal_generator_979 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 979, permutation);  generators.emplace_front(static_ideal_generator_979); \
	case 978:  static poly static_ideal_generator_978 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 978, permutation);  generators.emplace_front(static_ideal_generator_978); \
	case 977:  static poly static_ideal_generator_977 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 977, permutation);  generators.emplace_front(static_ideal_generator_977); \
	case 976:  static poly static_ideal_generator_976 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 976, permutation);  generators.emplace_front(static_ideal_generator_976); \
	case 975:  static poly static_ideal_generator_975 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 975, permutation);  generators.emplace_front(static_ideal_generator_975); \
	case 974:  static poly static_ideal_generator_974 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 974, permutation);  generators.emplace_front(static_ideal_generator_974); \
	case 973:  static poly static_ideal_generator_973 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 973, permutation);  generators.emplace_front(static_ideal_generator_973); \
	case 972:  static poly static_ideal_generator_972 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 972, permutation);  generators.emplace_front(static_ideal_generator_972); \
	case 971:  static poly static_ideal_generator_971 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 971, permutation);  generators.emplace_front(static_ideal_generator_971); \
	case 970:  static poly static_ideal_generator_970 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 970, permutation);  generators.emplace_front(static_ideal_generator_970); \
	case 969:  static poly static_ideal_generator_969 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 969, permutation);  generators.emplace_front(static_ideal_generator_969); \
	case 968:  static poly static_ideal_generator_968 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 968, permutation);  generators.emplace_front(static_ideal_generator_968); \
	case 967:  static poly static_ideal_generator_967 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 967, permutation);  generators.emplace_front(static_ideal_generator_967); \
	case 966:  static poly static_ideal_generator_966 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 966, permutation);  generators.emplace_front(static_ideal_generator_966); \
	case 965:  static poly static_ideal_generator_965 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 965, permutation);  generators.emplace_front(static_ideal_generator_965); \
	case 964:  static poly static_ideal_generator_964 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 964, permutation);  generators.emplace_front(static_ideal_generator_964); \
	case 963:  static poly static_ideal_generator_963 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 963, permutation);  generators.emplace_front(static_ideal_generator_963); \
	case 962:  static poly static_ideal_generator_962 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 962, permutation);  generators.emplace_front(static_ideal_generator_962); \
	case 961:  static poly static_ideal_generator_961 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 961, permutation);  generators.emplace_front(static_ideal_generator_961); \
	case 960:  static poly static_ideal_generator_960 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 960, permutation);  generators.emplace_front(static_ideal_generator_960); \
	case 959:  static poly static_ideal_generator_959 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 959, permutation);  generators.emplace_front(static_ideal_generator_959); \
	case 958:  static poly static_ideal_generator_958 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 958, permutation);  generators.emplace_front(static_ideal_generator_958); \
	case 957:  static poly static_ideal_generator_957 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 957, permutation);  generators.emplace_front(static_ideal_generator_957); \
	case 956:  static poly static_ideal_generator_956 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 956, permutation);  generators.emplace_front(static_ideal_generator_956); \
	case 955:  static poly static_ideal_generator_955 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 955, permutation);  generators.emplace_front(static_ideal_generator_955); \
	case 954:  static poly static_ideal_generator_954 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 954, permutation);  generators.emplace_front(static_ideal_generator_954); \
	case 953:  static poly static_ideal_generator_953 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 953, permutation);  generators.emplace_front(static_ideal_generator_953); \
	case 952:  static poly static_ideal_generator_952 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 952, permutation);  generators.emplace_front(static_ideal_generator_952); \
	case 951:  static poly static_ideal_generator_951 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 951, permutation);  generators.emplace_front(static_ideal_generator_951); \
	case 950:  static poly static_ideal_generator_950 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 950, permutation);  generators.emplace_front(static_ideal_generator_950); \
	case 949:  static poly static_ideal_generator_949 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 949, permutation);  generators.emplace_front(static_ideal_generator_949); \
	case 948:  static poly static_ideal_generator_948 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 948, permutation);  generators.emplace_front(static_ideal_generator_948); \
	case 947:  static poly static_ideal_generator_947 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 947, permutation);  generators.emplace_front(static_ideal_generator_947); \
	case 946:  static poly static_ideal_generator_946 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 946, permutation);  generators.emplace_front(static_ideal_generator_946); \
	case 945:  static poly static_ideal_generator_945 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 945, permutation);  generators.emplace_front(static_ideal_generator_945); \
	case 944:  static poly static_ideal_generator_944 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 944, permutation);  generators.emplace_front(static_ideal_generator_944); \
	case 943:  static poly static_ideal_generator_943 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 943, permutation);  generators.emplace_front(static_ideal_generator_943); \
	case 942:  static poly static_ideal_generator_942 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 942, permutation);  generators.emplace_front(static_ideal_generator_942); \
	case 941:  static poly static_ideal_generator_941 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 941, permutation);  generators.emplace_front(static_ideal_generator_941); \
	case 940:  static poly static_ideal_generator_940 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 940, permutation);  generators.emplace_front(static_ideal_generator_940); \
	case 939:  static poly static_ideal_generator_939 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 939, permutation);  generators.emplace_front(static_ideal_generator_939); \
	case 938:  static poly static_ideal_generator_938 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 938, permutation);  generators.emplace_front(static_ideal_generator_938); \
	case 937:  static poly static_ideal_generator_937 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 937, permutation);  generators.emplace_front(static_ideal_generator_937); \
	case 936:  static poly static_ideal_generator_936 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 936, permutation);  generators.emplace_front(static_ideal_generator_936); \
	case 935:  static poly static_ideal_generator_935 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 935, permutation);  generators.emplace_front(static_ideal_generator_935); \
	case 934:  static poly static_ideal_generator_934 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 934, permutation);  generators.emplace_front(static_ideal_generator_934); \
	case 933:  static poly static_ideal_generator_933 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 933, permutation);  generators.emplace_front(static_ideal_generator_933); \
	case 932:  static poly static_ideal_generator_932 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 932, permutation);  generators.emplace_front(static_ideal_generator_932); \
	case 931:  static poly static_ideal_generator_931 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 931, permutation);  generators.emplace_front(static_ideal_generator_931); \
	case 930:  static poly static_ideal_generator_930 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 930, permutation);  generators.emplace_front(static_ideal_generator_930); \
	case 929:  static poly static_ideal_generator_929 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 929, permutation);  generators.emplace_front(static_ideal_generator_929); \
	case 928:  static poly static_ideal_generator_928 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 928, permutation);  generators.emplace_front(static_ideal_generator_928); \
	case 927:  static poly static_ideal_generator_927 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 927, permutation);  generators.emplace_front(static_ideal_generator_927); \
	case 926:  static poly static_ideal_generator_926 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 926, permutation);  generators.emplace_front(static_ideal_generator_926); \
	case 925:  static poly static_ideal_generator_925 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 925, permutation);  generators.emplace_front(static_ideal_generator_925); \
	case 924:  static poly static_ideal_generator_924 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 924, permutation);  generators.emplace_front(static_ideal_generator_924); \
	case 923:  static poly static_ideal_generator_923 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 923, permutation);  generators.emplace_front(static_ideal_generator_923); \
	case 922:  static poly static_ideal_generator_922 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 922, permutation);  generators.emplace_front(static_ideal_generator_922); \
	case 921:  static poly static_ideal_generator_921 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 921, permutation);  generators.emplace_front(static_ideal_generator_921); \
	case 920:  static poly static_ideal_generator_920 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 920, permutation);  generators.emplace_front(static_ideal_generator_920); \
	case 919:  static poly static_ideal_generator_919 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 919, permutation);  generators.emplace_front(static_ideal_generator_919); \
	case 918:  static poly static_ideal_generator_918 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 918, permutation);  generators.emplace_front(static_ideal_generator_918); \
	case 917:  static poly static_ideal_generator_917 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 917, permutation);  generators.emplace_front(static_ideal_generator_917); \
	case 916:  static poly static_ideal_generator_916 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 916, permutation);  generators.emplace_front(static_ideal_generator_916); \
	case 915:  static poly static_ideal_generator_915 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 915, permutation);  generators.emplace_front(static_ideal_generator_915); \
	case 914:  static poly static_ideal_generator_914 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 914, permutation);  generators.emplace_front(static_ideal_generator_914); \
	case 913:  static poly static_ideal_generator_913 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 913, permutation);  generators.emplace_front(static_ideal_generator_913); \
	case 912:  static poly static_ideal_generator_912 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 912, permutation);  generators.emplace_front(static_ideal_generator_912); \
	case 911:  static poly static_ideal_generator_911 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 911, permutation);  generators.emplace_front(static_ideal_generator_911); \
	case 910:  static poly static_ideal_generator_910 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 910, permutation);  generators.emplace_front(static_ideal_generator_910); \
	case 909:  static poly static_ideal_generator_909 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 909, permutation);  generators.emplace_front(static_ideal_generator_909); \
	case 908:  static poly static_ideal_generator_908 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 908, permutation);  generators.emplace_front(static_ideal_generator_908); \
	case 907:  static poly static_ideal_generator_907 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 907, permutation);  generators.emplace_front(static_ideal_generator_907); \
	case 906:  static poly static_ideal_generator_906 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 906, permutation);  generators.emplace_front(static_ideal_generator_906); \
	case 905:  static poly static_ideal_generator_905 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 905, permutation);  generators.emplace_front(static_ideal_generator_905); \
	case 904:  static poly static_ideal_generator_904 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 904, permutation);  generators.emplace_front(static_ideal_generator_904); \
	case 903:  static poly static_ideal_generator_903 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 903, permutation);  generators.emplace_front(static_ideal_generator_903); \
	case 902:  static poly static_ideal_generator_902 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 902, permutation);  generators.emplace_front(static_ideal_generator_902); \
	case 901:  static poly static_ideal_generator_901 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 901, permutation);  generators.emplace_front(static_ideal_generator_901); \
	case 900:  static poly static_ideal_generator_900 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 900, permutation);  generators.emplace_front(static_ideal_generator_900); \
	case 899:  static poly static_ideal_generator_899 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 899, permutation);  generators.emplace_front(static_ideal_generator_899); \
	case 898:  static poly static_ideal_generator_898 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 898, permutation);  generators.emplace_front(static_ideal_generator_898); \
	case 897:  static poly static_ideal_generator_897 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 897, permutation);  generators.emplace_front(static_ideal_generator_897); \
	case 896:  static poly static_ideal_generator_896 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 896, permutation);  generators.emplace_front(static_ideal_generator_896); \
	case 895:  static poly static_ideal_generator_895 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 895, permutation);  generators.emplace_front(static_ideal_generator_895); \
	case 894:  static poly static_ideal_generator_894 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 894, permutation);  generators.emplace_front(static_ideal_generator_894); \
	case 893:  static poly static_ideal_generator_893 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 893, permutation);  generators.emplace_front(static_ideal_generator_893); \
	case 892:  static poly static_ideal_generator_892 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 892, permutation);  generators.emplace_front(static_ideal_generator_892); \
	case 891:  static poly static_ideal_generator_891 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 891, permutation);  generators.emplace_front(static_ideal_generator_891); \
	case 890:  static poly static_ideal_generator_890 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 890, permutation);  generators.emplace_front(static_ideal_generator_890); \
	case 889:  static poly static_ideal_generator_889 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 889, permutation);  generators.emplace_front(static_ideal_generator_889); \
	case 888:  static poly static_ideal_generator_888 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 888, permutation);  generators.emplace_front(static_ideal_generator_888); \
	case 887:  static poly static_ideal_generator_887 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 887, permutation);  generators.emplace_front(static_ideal_generator_887); \
	case 886:  static poly static_ideal_generator_886 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 886, permutation);  generators.emplace_front(static_ideal_generator_886); \
	case 885:  static poly static_ideal_generator_885 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 885, permutation);  generators.emplace_front(static_ideal_generator_885); \
	case 884:  static poly static_ideal_generator_884 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 884, permutation);  generators.emplace_front(static_ideal_generator_884); \
	case 883:  static poly static_ideal_generator_883 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 883, permutation);  generators.emplace_front(static_ideal_generator_883); \
	case 882:  static poly static_ideal_generator_882 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 882, permutation);  generators.emplace_front(static_ideal_generator_882); \
	case 881:  static poly static_ideal_generator_881 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 881, permutation);  generators.emplace_front(static_ideal_generator_881); \
	case 880:  static poly static_ideal_generator_880 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 880, permutation);  generators.emplace_front(static_ideal_generator_880); \
	case 879:  static poly static_ideal_generator_879 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 879, permutation);  generators.emplace_front(static_ideal_generator_879); \
	case 878:  static poly static_ideal_generator_878 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 878, permutation);  generators.emplace_front(static_ideal_generator_878); \
	case 877:  static poly static_ideal_generator_877 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 877, permutation);  generators.emplace_front(static_ideal_generator_877); \
	case 876:  static poly static_ideal_generator_876 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 876, permutation);  generators.emplace_front(static_ideal_generator_876); \
	case 875:  static poly static_ideal_generator_875 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 875, permutation);  generators.emplace_front(static_ideal_generator_875); \
	case 874:  static poly static_ideal_generator_874 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 874, permutation);  generators.emplace_front(static_ideal_generator_874); \
	case 873:  static poly static_ideal_generator_873 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 873, permutation);  generators.emplace_front(static_ideal_generator_873); \
	case 872:  static poly static_ideal_generator_872 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 872, permutation);  generators.emplace_front(static_ideal_generator_872); \
	case 871:  static poly static_ideal_generator_871 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 871, permutation);  generators.emplace_front(static_ideal_generator_871); \
	case 870:  static poly static_ideal_generator_870 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 870, permutation);  generators.emplace_front(static_ideal_generator_870); \
	case 869:  static poly static_ideal_generator_869 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 869, permutation);  generators.emplace_front(static_ideal_generator_869); \
	case 868:  static poly static_ideal_generator_868 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 868, permutation);  generators.emplace_front(static_ideal_generator_868); \
	case 867:  static poly static_ideal_generator_867 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 867, permutation);  generators.emplace_front(static_ideal_generator_867); \
	case 866:  static poly static_ideal_generator_866 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 866, permutation);  generators.emplace_front(static_ideal_generator_866); \
	case 865:  static poly static_ideal_generator_865 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 865, permutation);  generators.emplace_front(static_ideal_generator_865); \
	case 864:  static poly static_ideal_generator_864 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 864, permutation);  generators.emplace_front(static_ideal_generator_864); \
	case 863:  static poly static_ideal_generator_863 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 863, permutation);  generators.emplace_front(static_ideal_generator_863); \
	case 862:  static poly static_ideal_generator_862 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 862, permutation);  generators.emplace_front(static_ideal_generator_862); \
	case 861:  static poly static_ideal_generator_861 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 861, permutation);  generators.emplace_front(static_ideal_generator_861); \
	case 860:  static poly static_ideal_generator_860 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 860, permutation);  generators.emplace_front(static_ideal_generator_860); \
	case 859:  static poly static_ideal_generator_859 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 859, permutation);  generators.emplace_front(static_ideal_generator_859); \
	case 858:  static poly static_ideal_generator_858 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 858, permutation);  generators.emplace_front(static_ideal_generator_858); \
	case 857:  static poly static_ideal_generator_857 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 857, permutation);  generators.emplace_front(static_ideal_generator_857); \
	case 856:  static poly static_ideal_generator_856 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 856, permutation);  generators.emplace_front(static_ideal_generator_856); \
	case 855:  static poly static_ideal_generator_855 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 855, permutation);  generators.emplace_front(static_ideal_generator_855); \
	case 854:  static poly static_ideal_generator_854 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 854, permutation);  generators.emplace_front(static_ideal_generator_854); \
	case 853:  static poly static_ideal_generator_853 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 853, permutation);  generators.emplace_front(static_ideal_generator_853); \
	case 852:  static poly static_ideal_generator_852 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 852, permutation);  generators.emplace_front(static_ideal_generator_852); \
	case 851:  static poly static_ideal_generator_851 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 851, permutation);  generators.emplace_front(static_ideal_generator_851); \
	case 850:  static poly static_ideal_generator_850 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 850, permutation);  generators.emplace_front(static_ideal_generator_850); \
	case 849:  static poly static_ideal_generator_849 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 849, permutation);  generators.emplace_front(static_ideal_generator_849); \
	case 848:  static poly static_ideal_generator_848 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 848, permutation);  generators.emplace_front(static_ideal_generator_848); \
	case 847:  static poly static_ideal_generator_847 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 847, permutation);  generators.emplace_front(static_ideal_generator_847); \
	case 846:  static poly static_ideal_generator_846 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 846, permutation);  generators.emplace_front(static_ideal_generator_846); \
	case 845:  static poly static_ideal_generator_845 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 845, permutation);  generators.emplace_front(static_ideal_generator_845); \
	case 844:  static poly static_ideal_generator_844 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 844, permutation);  generators.emplace_front(static_ideal_generator_844); \
	case 843:  static poly static_ideal_generator_843 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 843, permutation);  generators.emplace_front(static_ideal_generator_843); \
	case 842:  static poly static_ideal_generator_842 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 842, permutation);  generators.emplace_front(static_ideal_generator_842); \
	case 841:  static poly static_ideal_generator_841 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 841, permutation);  generators.emplace_front(static_ideal_generator_841); \
	case 840:  static poly static_ideal_generator_840 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 840, permutation);  generators.emplace_front(static_ideal_generator_840); \
	case 839:  static poly static_ideal_generator_839 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 839, permutation);  generators.emplace_front(static_ideal_generator_839); \
	case 838:  static poly static_ideal_generator_838 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 838, permutation);  generators.emplace_front(static_ideal_generator_838); \
	case 837:  static poly static_ideal_generator_837 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 837, permutation);  generators.emplace_front(static_ideal_generator_837); \
	case 836:  static poly static_ideal_generator_836 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 836, permutation);  generators.emplace_front(static_ideal_generator_836); \
	case 835:  static poly static_ideal_generator_835 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 835, permutation);  generators.emplace_front(static_ideal_generator_835); \
	case 834:  static poly static_ideal_generator_834 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 834, permutation);  generators.emplace_front(static_ideal_generator_834); \
	case 833:  static poly static_ideal_generator_833 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 833, permutation);  generators.emplace_front(static_ideal_generator_833); \
	case 832:  static poly static_ideal_generator_832 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 832, permutation);  generators.emplace_front(static_ideal_generator_832); \
	case 831:  static poly static_ideal_generator_831 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 831, permutation);  generators.emplace_front(static_ideal_generator_831); \
	case 830:  static poly static_ideal_generator_830 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 830, permutation);  generators.emplace_front(static_ideal_generator_830); \
	case 829:  static poly static_ideal_generator_829 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 829, permutation);  generators.emplace_front(static_ideal_generator_829); \
	case 828:  static poly static_ideal_generator_828 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 828, permutation);  generators.emplace_front(static_ideal_generator_828); \
	case 827:  static poly static_ideal_generator_827 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 827, permutation);  generators.emplace_front(static_ideal_generator_827); \
	case 826:  static poly static_ideal_generator_826 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 826, permutation);  generators.emplace_front(static_ideal_generator_826); \
	case 825:  static poly static_ideal_generator_825 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 825, permutation);  generators.emplace_front(static_ideal_generator_825); \
	case 824:  static poly static_ideal_generator_824 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 824, permutation);  generators.emplace_front(static_ideal_generator_824); \
	case 823:  static poly static_ideal_generator_823 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 823, permutation);  generators.emplace_front(static_ideal_generator_823); \
	case 822:  static poly static_ideal_generator_822 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 822, permutation);  generators.emplace_front(static_ideal_generator_822); \
	case 821:  static poly static_ideal_generator_821 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 821, permutation);  generators.emplace_front(static_ideal_generator_821); \
	case 820:  static poly static_ideal_generator_820 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 820, permutation);  generators.emplace_front(static_ideal_generator_820); \
	case 819:  static poly static_ideal_generator_819 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 819, permutation);  generators.emplace_front(static_ideal_generator_819); \
	case 818:  static poly static_ideal_generator_818 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 818, permutation);  generators.emplace_front(static_ideal_generator_818); \
	case 817:  static poly static_ideal_generator_817 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 817, permutation);  generators.emplace_front(static_ideal_generator_817); \
	case 816:  static poly static_ideal_generator_816 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 816, permutation);  generators.emplace_front(static_ideal_generator_816); \
	case 815:  static poly static_ideal_generator_815 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 815, permutation);  generators.emplace_front(static_ideal_generator_815); \
	case 814:  static poly static_ideal_generator_814 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 814, permutation);  generators.emplace_front(static_ideal_generator_814); \
	case 813:  static poly static_ideal_generator_813 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 813, permutation);  generators.emplace_front(static_ideal_generator_813); \
	case 812:  static poly static_ideal_generator_812 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 812, permutation);  generators.emplace_front(static_ideal_generator_812); \
	case 811:  static poly static_ideal_generator_811 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 811, permutation);  generators.emplace_front(static_ideal_generator_811); \
	case 810:  static poly static_ideal_generator_810 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 810, permutation);  generators.emplace_front(static_ideal_generator_810); \
	case 809:  static poly static_ideal_generator_809 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 809, permutation);  generators.emplace_front(static_ideal_generator_809); \
	case 808:  static poly static_ideal_generator_808 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 808, permutation);  generators.emplace_front(static_ideal_generator_808); \
	case 807:  static poly static_ideal_generator_807 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 807, permutation);  generators.emplace_front(static_ideal_generator_807); \
	case 806:  static poly static_ideal_generator_806 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 806, permutation);  generators.emplace_front(static_ideal_generator_806); \
	case 805:  static poly static_ideal_generator_805 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 805, permutation);  generators.emplace_front(static_ideal_generator_805); \
	case 804:  static poly static_ideal_generator_804 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 804, permutation);  generators.emplace_front(static_ideal_generator_804); \
	case 803:  static poly static_ideal_generator_803 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 803, permutation);  generators.emplace_front(static_ideal_generator_803); \
	case 802:  static poly static_ideal_generator_802 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 802, permutation);  generators.emplace_front(static_ideal_generator_802); \
	case 801:  static poly static_ideal_generator_801 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 801, permutation);  generators.emplace_front(static_ideal_generator_801); \
	case 800:  static poly static_ideal_generator_800 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 800, permutation);  generators.emplace_front(static_ideal_generator_800); \
	case 799:  static poly static_ideal_generator_799 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 799, permutation);  generators.emplace_front(static_ideal_generator_799); \
	case 798:  static poly static_ideal_generator_798 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 798, permutation);  generators.emplace_front(static_ideal_generator_798); \
	case 797:  static poly static_ideal_generator_797 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 797, permutation);  generators.emplace_front(static_ideal_generator_797); \
	case 796:  static poly static_ideal_generator_796 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 796, permutation);  generators.emplace_front(static_ideal_generator_796); \
	case 795:  static poly static_ideal_generator_795 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 795, permutation);  generators.emplace_front(static_ideal_generator_795); \
	case 794:  static poly static_ideal_generator_794 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 794, permutation);  generators.emplace_front(static_ideal_generator_794); \
	case 793:  static poly static_ideal_generator_793 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 793, permutation);  generators.emplace_front(static_ideal_generator_793); \
	case 792:  static poly static_ideal_generator_792 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 792, permutation);  generators.emplace_front(static_ideal_generator_792); \
	case 791:  static poly static_ideal_generator_791 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 791, permutation);  generators.emplace_front(static_ideal_generator_791); \
	case 790:  static poly static_ideal_generator_790 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 790, permutation);  generators.emplace_front(static_ideal_generator_790); \
	case 789:  static poly static_ideal_generator_789 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 789, permutation);  generators.emplace_front(static_ideal_generator_789); \
	case 788:  static poly static_ideal_generator_788 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 788, permutation);  generators.emplace_front(static_ideal_generator_788); \
	case 787:  static poly static_ideal_generator_787 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 787, permutation);  generators.emplace_front(static_ideal_generator_787); \
	case 786:  static poly static_ideal_generator_786 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 786, permutation);  generators.emplace_front(static_ideal_generator_786); \
	case 785:  static poly static_ideal_generator_785 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 785, permutation);  generators.emplace_front(static_ideal_generator_785); \
	case 784:  static poly static_ideal_generator_784 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 784, permutation);  generators.emplace_front(static_ideal_generator_784); \
	case 783:  static poly static_ideal_generator_783 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 783, permutation);  generators.emplace_front(static_ideal_generator_783); \
	case 782:  static poly static_ideal_generator_782 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 782, permutation);  generators.emplace_front(static_ideal_generator_782); \
	case 781:  static poly static_ideal_generator_781 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 781, permutation);  generators.emplace_front(static_ideal_generator_781); \
	case 780:  static poly static_ideal_generator_780 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 780, permutation);  generators.emplace_front(static_ideal_generator_780); \
	case 779:  static poly static_ideal_generator_779 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 779, permutation);  generators.emplace_front(static_ideal_generator_779); \
	case 778:  static poly static_ideal_generator_778 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 778, permutation);  generators.emplace_front(static_ideal_generator_778); \
	case 777:  static poly static_ideal_generator_777 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 777, permutation);  generators.emplace_front(static_ideal_generator_777); \
	case 776:  static poly static_ideal_generator_776 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 776, permutation);  generators.emplace_front(static_ideal_generator_776); \
	case 775:  static poly static_ideal_generator_775 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 775, permutation);  generators.emplace_front(static_ideal_generator_775); \
	case 774:  static poly static_ideal_generator_774 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 774, permutation);  generators.emplace_front(static_ideal_generator_774); \
	case 773:  static poly static_ideal_generator_773 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 773, permutation);  generators.emplace_front(static_ideal_generator_773); \
	case 772:  static poly static_ideal_generator_772 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 772, permutation);  generators.emplace_front(static_ideal_generator_772); \
	case 771:  static poly static_ideal_generator_771 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 771, permutation);  generators.emplace_front(static_ideal_generator_771); \
	case 770:  static poly static_ideal_generator_770 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 770, permutation);  generators.emplace_front(static_ideal_generator_770); \
	case 769:  static poly static_ideal_generator_769 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 769, permutation);  generators.emplace_front(static_ideal_generator_769); \
	case 768:  static poly static_ideal_generator_768 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 768, permutation);  generators.emplace_front(static_ideal_generator_768); \
	case 767:  static poly static_ideal_generator_767 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 767, permutation);  generators.emplace_front(static_ideal_generator_767); \
	case 766:  static poly static_ideal_generator_766 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 766, permutation);  generators.emplace_front(static_ideal_generator_766); \
	case 765:  static poly static_ideal_generator_765 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 765, permutation);  generators.emplace_front(static_ideal_generator_765); \
	case 764:  static poly static_ideal_generator_764 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 764, permutation);  generators.emplace_front(static_ideal_generator_764); \
	case 763:  static poly static_ideal_generator_763 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 763, permutation);  generators.emplace_front(static_ideal_generator_763); \
	case 762:  static poly static_ideal_generator_762 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 762, permutation);  generators.emplace_front(static_ideal_generator_762); \
	case 761:  static poly static_ideal_generator_761 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 761, permutation);  generators.emplace_front(static_ideal_generator_761); \
	case 760:  static poly static_ideal_generator_760 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 760, permutation);  generators.emplace_front(static_ideal_generator_760); \
	case 759:  static poly static_ideal_generator_759 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 759, permutation);  generators.emplace_front(static_ideal_generator_759); \
	case 758:  static poly static_ideal_generator_758 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 758, permutation);  generators.emplace_front(static_ideal_generator_758); \
	case 757:  static poly static_ideal_generator_757 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 757, permutation);  generators.emplace_front(static_ideal_generator_757); \
	case 756:  static poly static_ideal_generator_756 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 756, permutation);  generators.emplace_front(static_ideal_generator_756); \
	case 755:  static poly static_ideal_generator_755 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 755, permutation);  generators.emplace_front(static_ideal_generator_755); \
	case 754:  static poly static_ideal_generator_754 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 754, permutation);  generators.emplace_front(static_ideal_generator_754); \
	case 753:  static poly static_ideal_generator_753 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 753, permutation);  generators.emplace_front(static_ideal_generator_753); \
	case 752:  static poly static_ideal_generator_752 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 752, permutation);  generators.emplace_front(static_ideal_generator_752); \
	case 751:  static poly static_ideal_generator_751 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 751, permutation);  generators.emplace_front(static_ideal_generator_751); \
	case 750:  static poly static_ideal_generator_750 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 750, permutation);  generators.emplace_front(static_ideal_generator_750); \
	case 749:  static poly static_ideal_generator_749 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 749, permutation);  generators.emplace_front(static_ideal_generator_749); \
	case 748:  static poly static_ideal_generator_748 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 748, permutation);  generators.emplace_front(static_ideal_generator_748); \
	case 747:  static poly static_ideal_generator_747 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 747, permutation);  generators.emplace_front(static_ideal_generator_747); \
	case 746:  static poly static_ideal_generator_746 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 746, permutation);  generators.emplace_front(static_ideal_generator_746); \
	case 745:  static poly static_ideal_generator_745 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 745, permutation);  generators.emplace_front(static_ideal_generator_745); \
	case 744:  static poly static_ideal_generator_744 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 744, permutation);  generators.emplace_front(static_ideal_generator_744); \
	case 743:  static poly static_ideal_generator_743 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 743, permutation);  generators.emplace_front(static_ideal_generator_743); \
	case 742:  static poly static_ideal_generator_742 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 742, permutation);  generators.emplace_front(static_ideal_generator_742); \
	case 741:  static poly static_ideal_generator_741 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 741, permutation);  generators.emplace_front(static_ideal_generator_741); \
	case 740:  static poly static_ideal_generator_740 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 740, permutation);  generators.emplace_front(static_ideal_generator_740); \
	case 739:  static poly static_ideal_generator_739 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 739, permutation);  generators.emplace_front(static_ideal_generator_739); \
	case 738:  static poly static_ideal_generator_738 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 738, permutation);  generators.emplace_front(static_ideal_generator_738); \
	case 737:  static poly static_ideal_generator_737 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 737, permutation);  generators.emplace_front(static_ideal_generator_737); \
	case 736:  static poly static_ideal_generator_736 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 736, permutation);  generators.emplace_front(static_ideal_generator_736); \
	case 735:  static poly static_ideal_generator_735 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 735, permutation);  generators.emplace_front(static_ideal_generator_735); \
	case 734:  static poly static_ideal_generator_734 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 734, permutation);  generators.emplace_front(static_ideal_generator_734); \
	case 733:  static poly static_ideal_generator_733 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 733, permutation);  generators.emplace_front(static_ideal_generator_733); \
	case 732:  static poly static_ideal_generator_732 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 732, permutation);  generators.emplace_front(static_ideal_generator_732); \
	case 731:  static poly static_ideal_generator_731 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 731, permutation);  generators.emplace_front(static_ideal_generator_731); \
	case 730:  static poly static_ideal_generator_730 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 730, permutation);  generators.emplace_front(static_ideal_generator_730); \
	case 729:  static poly static_ideal_generator_729 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 729, permutation);  generators.emplace_front(static_ideal_generator_729); \
	case 728:  static poly static_ideal_generator_728 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 728, permutation);  generators.emplace_front(static_ideal_generator_728); \
	case 727:  static poly static_ideal_generator_727 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 727, permutation);  generators.emplace_front(static_ideal_generator_727); \
	case 726:  static poly static_ideal_generator_726 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 726, permutation);  generators.emplace_front(static_ideal_generator_726); \
	case 725:  static poly static_ideal_generator_725 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 725, permutation);  generators.emplace_front(static_ideal_generator_725); \
	case 724:  static poly static_ideal_generator_724 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 724, permutation);  generators.emplace_front(static_ideal_generator_724); \
	case 723:  static poly static_ideal_generator_723 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 723, permutation);  generators.emplace_front(static_ideal_generator_723); \
	case 722:  static poly static_ideal_generator_722 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 722, permutation);  generators.emplace_front(static_ideal_generator_722); \
	case 721:  static poly static_ideal_generator_721 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 721, permutation);  generators.emplace_front(static_ideal_generator_721); \
	case 720:  static poly static_ideal_generator_720 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 720, permutation);  generators.emplace_front(static_ideal_generator_720); \
	case 719:  static poly static_ideal_generator_719 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 719, permutation);  generators.emplace_front(static_ideal_generator_719); \
	case 718:  static poly static_ideal_generator_718 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 718, permutation);  generators.emplace_front(static_ideal_generator_718); \
	case 717:  static poly static_ideal_generator_717 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 717, permutation);  generators.emplace_front(static_ideal_generator_717); \
	case 716:  static poly static_ideal_generator_716 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 716, permutation);  generators.emplace_front(static_ideal_generator_716); \
	case 715:  static poly static_ideal_generator_715 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 715, permutation);  generators.emplace_front(static_ideal_generator_715); \
	case 714:  static poly static_ideal_generator_714 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 714, permutation);  generators.emplace_front(static_ideal_generator_714); \
	case 713:  static poly static_ideal_generator_713 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 713, permutation);  generators.emplace_front(static_ideal_generator_713); \
	case 712:  static poly static_ideal_generator_712 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 712, permutation);  generators.emplace_front(static_ideal_generator_712); \
	case 711:  static poly static_ideal_generator_711 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 711, permutation);  generators.emplace_front(static_ideal_generator_711); \
	case 710:  static poly static_ideal_generator_710 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 710, permutation);  generators.emplace_front(static_ideal_generator_710); \
	case 709:  static poly static_ideal_generator_709 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 709, permutation);  generators.emplace_front(static_ideal_generator_709); \
	case 708:  static poly static_ideal_generator_708 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 708, permutation);  generators.emplace_front(static_ideal_generator_708); \
	case 707:  static poly static_ideal_generator_707 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 707, permutation);  generators.emplace_front(static_ideal_generator_707); \
	case 706:  static poly static_ideal_generator_706 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 706, permutation);  generators.emplace_front(static_ideal_generator_706); \
	case 705:  static poly static_ideal_generator_705 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 705, permutation);  generators.emplace_front(static_ideal_generator_705); \
	case 704:  static poly static_ideal_generator_704 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 704, permutation);  generators.emplace_front(static_ideal_generator_704); \
	case 703:  static poly static_ideal_generator_703 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 703, permutation);  generators.emplace_front(static_ideal_generator_703); \
	case 702:  static poly static_ideal_generator_702 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 702, permutation);  generators.emplace_front(static_ideal_generator_702); \
	case 701:  static poly static_ideal_generator_701 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 701, permutation);  generators.emplace_front(static_ideal_generator_701); \
	case 700:  static poly static_ideal_generator_700 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 700, permutation);  generators.emplace_front(static_ideal_generator_700); \
	case 699:  static poly static_ideal_generator_699 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 699, permutation);  generators.emplace_front(static_ideal_generator_699); \
	case 698:  static poly static_ideal_generator_698 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 698, permutation);  generators.emplace_front(static_ideal_generator_698); \
	case 697:  static poly static_ideal_generator_697 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 697, permutation);  generators.emplace_front(static_ideal_generator_697); \
	case 696:  static poly static_ideal_generator_696 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 696, permutation);  generators.emplace_front(static_ideal_generator_696); \
	case 695:  static poly static_ideal_generator_695 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 695, permutation);  generators.emplace_front(static_ideal_generator_695); \
	case 694:  static poly static_ideal_generator_694 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 694, permutation);  generators.emplace_front(static_ideal_generator_694); \
	case 693:  static poly static_ideal_generator_693 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 693, permutation);  generators.emplace_front(static_ideal_generator_693); \
	case 692:  static poly static_ideal_generator_692 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 692, permutation);  generators.emplace_front(static_ideal_generator_692); \
	case 691:  static poly static_ideal_generator_691 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 691, permutation);  generators.emplace_front(static_ideal_generator_691); \
	case 690:  static poly static_ideal_generator_690 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 690, permutation);  generators.emplace_front(static_ideal_generator_690); \
	case 689:  static poly static_ideal_generator_689 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 689, permutation);  generators.emplace_front(static_ideal_generator_689); \
	case 688:  static poly static_ideal_generator_688 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 688, permutation);  generators.emplace_front(static_ideal_generator_688); \
	case 687:  static poly static_ideal_generator_687 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 687, permutation);  generators.emplace_front(static_ideal_generator_687); \
	case 686:  static poly static_ideal_generator_686 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 686, permutation);  generators.emplace_front(static_ideal_generator_686); \
	case 685:  static poly static_ideal_generator_685 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 685, permutation);  generators.emplace_front(static_ideal_generator_685); \
	case 684:  static poly static_ideal_generator_684 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 684, permutation);  generators.emplace_front(static_ideal_generator_684); \
	case 683:  static poly static_ideal_generator_683 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 683, permutation);  generators.emplace_front(static_ideal_generator_683); \
	case 682:  static poly static_ideal_generator_682 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 682, permutation);  generators.emplace_front(static_ideal_generator_682); \
	case 681:  static poly static_ideal_generator_681 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 681, permutation);  generators.emplace_front(static_ideal_generator_681); \
	case 680:  static poly static_ideal_generator_680 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 680, permutation);  generators.emplace_front(static_ideal_generator_680); \
	case 679:  static poly static_ideal_generator_679 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 679, permutation);  generators.emplace_front(static_ideal_generator_679); \
	case 678:  static poly static_ideal_generator_678 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 678, permutation);  generators.emplace_front(static_ideal_generator_678); \
	case 677:  static poly static_ideal_generator_677 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 677, permutation);  generators.emplace_front(static_ideal_generator_677); \
	case 676:  static poly static_ideal_generator_676 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 676, permutation);  generators.emplace_front(static_ideal_generator_676); \
	case 675:  static poly static_ideal_generator_675 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 675, permutation);  generators.emplace_front(static_ideal_generator_675); \
	case 674:  static poly static_ideal_generator_674 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 674, permutation);  generators.emplace_front(static_ideal_generator_674); \
	case 673:  static poly static_ideal_generator_673 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 673, permutation);  generators.emplace_front(static_ideal_generator_673); \
	case 672:  static poly static_ideal_generator_672 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 672, permutation);  generators.emplace_front(static_ideal_generator_672); \
	case 671:  static poly static_ideal_generator_671 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 671, permutation);  generators.emplace_front(static_ideal_generator_671); \
	case 670:  static poly static_ideal_generator_670 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 670, permutation);  generators.emplace_front(static_ideal_generator_670); \
	case 669:  static poly static_ideal_generator_669 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 669, permutation);  generators.emplace_front(static_ideal_generator_669); \
	case 668:  static poly static_ideal_generator_668 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 668, permutation);  generators.emplace_front(static_ideal_generator_668); \
	case 667:  static poly static_ideal_generator_667 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 667, permutation);  generators.emplace_front(static_ideal_generator_667); \
	case 666:  static poly static_ideal_generator_666 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 666, permutation);  generators.emplace_front(static_ideal_generator_666); \
	case 665:  static poly static_ideal_generator_665 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 665, permutation);  generators.emplace_front(static_ideal_generator_665); \
	case 664:  static poly static_ideal_generator_664 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 664, permutation);  generators.emplace_front(static_ideal_generator_664); \
	case 663:  static poly static_ideal_generator_663 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 663, permutation);  generators.emplace_front(static_ideal_generator_663); \
	case 662:  static poly static_ideal_generator_662 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 662, permutation);  generators.emplace_front(static_ideal_generator_662); \
	case 661:  static poly static_ideal_generator_661 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 661, permutation);  generators.emplace_front(static_ideal_generator_661); \
	case 660:  static poly static_ideal_generator_660 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 660, permutation);  generators.emplace_front(static_ideal_generator_660); \
	case 659:  static poly static_ideal_generator_659 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 659, permutation);  generators.emplace_front(static_ideal_generator_659); \
	case 658:  static poly static_ideal_generator_658 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 658, permutation);  generators.emplace_front(static_ideal_generator_658); \
	case 657:  static poly static_ideal_generator_657 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 657, permutation);  generators.emplace_front(static_ideal_generator_657); \
	case 656:  static poly static_ideal_generator_656 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 656, permutation);  generators.emplace_front(static_ideal_generator_656); \
	case 655:  static poly static_ideal_generator_655 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 655, permutation);  generators.emplace_front(static_ideal_generator_655); \
	case 654:  static poly static_ideal_generator_654 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 654, permutation);  generators.emplace_front(static_ideal_generator_654); \
	case 653:  static poly static_ideal_generator_653 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 653, permutation);  generators.emplace_front(static_ideal_generator_653); \
	case 652:  static poly static_ideal_generator_652 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 652, permutation);  generators.emplace_front(static_ideal_generator_652); \
	case 651:  static poly static_ideal_generator_651 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 651, permutation);  generators.emplace_front(static_ideal_generator_651); \
	case 650:  static poly static_ideal_generator_650 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 650, permutation);  generators.emplace_front(static_ideal_generator_650); \
	case 649:  static poly static_ideal_generator_649 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 649, permutation);  generators.emplace_front(static_ideal_generator_649); \
	case 648:  static poly static_ideal_generator_648 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 648, permutation);  generators.emplace_front(static_ideal_generator_648); \
	case 647:  static poly static_ideal_generator_647 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 647, permutation);  generators.emplace_front(static_ideal_generator_647); \
	case 646:  static poly static_ideal_generator_646 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 646, permutation);  generators.emplace_front(static_ideal_generator_646); \
	case 645:  static poly static_ideal_generator_645 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 645, permutation);  generators.emplace_front(static_ideal_generator_645); \
	case 644:  static poly static_ideal_generator_644 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 644, permutation);  generators.emplace_front(static_ideal_generator_644); \
	case 643:  static poly static_ideal_generator_643 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 643, permutation);  generators.emplace_front(static_ideal_generator_643); \
	case 642:  static poly static_ideal_generator_642 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 642, permutation);  generators.emplace_front(static_ideal_generator_642); \
	case 641:  static poly static_ideal_generator_641 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 641, permutation);  generators.emplace_front(static_ideal_generator_641); \
	case 640:  static poly static_ideal_generator_640 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 640, permutation);  generators.emplace_front(static_ideal_generator_640); \
	case 639:  static poly static_ideal_generator_639 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 639, permutation);  generators.emplace_front(static_ideal_generator_639); \
	case 638:  static poly static_ideal_generator_638 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 638, permutation);  generators.emplace_front(static_ideal_generator_638); \
	case 637:  static poly static_ideal_generator_637 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 637, permutation);  generators.emplace_front(static_ideal_generator_637); \
	case 636:  static poly static_ideal_generator_636 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 636, permutation);  generators.emplace_front(static_ideal_generator_636); \
	case 635:  static poly static_ideal_generator_635 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 635, permutation);  generators.emplace_front(static_ideal_generator_635); \
	case 634:  static poly static_ideal_generator_634 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 634, permutation);  generators.emplace_front(static_ideal_generator_634); \
	case 633:  static poly static_ideal_generator_633 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 633, permutation);  generators.emplace_front(static_ideal_generator_633); \
	case 632:  static poly static_ideal_generator_632 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 632, permutation);  generators.emplace_front(static_ideal_generator_632); \
	case 631:  static poly static_ideal_generator_631 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 631, permutation);  generators.emplace_front(static_ideal_generator_631); \
	case 630:  static poly static_ideal_generator_630 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 630, permutation);  generators.emplace_front(static_ideal_generator_630); \
	case 629:  static poly static_ideal_generator_629 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 629, permutation);  generators.emplace_front(static_ideal_generator_629); \
	case 628:  static poly static_ideal_generator_628 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 628, permutation);  generators.emplace_front(static_ideal_generator_628); \
	case 627:  static poly static_ideal_generator_627 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 627, permutation);  generators.emplace_front(static_ideal_generator_627); \
	case 626:  static poly static_ideal_generator_626 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 626, permutation);  generators.emplace_front(static_ideal_generator_626); \
	case 625:  static poly static_ideal_generator_625 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 625, permutation);  generators.emplace_front(static_ideal_generator_625); \
	case 624:  static poly static_ideal_generator_624 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 624, permutation);  generators.emplace_front(static_ideal_generator_624); \
	case 623:  static poly static_ideal_generator_623 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 623, permutation);  generators.emplace_front(static_ideal_generator_623); \
	case 622:  static poly static_ideal_generator_622 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 622, permutation);  generators.emplace_front(static_ideal_generator_622); \
	case 621:  static poly static_ideal_generator_621 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 621, permutation);  generators.emplace_front(static_ideal_generator_621); \
	case 620:  static poly static_ideal_generator_620 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 620, permutation);  generators.emplace_front(static_ideal_generator_620); \
	case 619:  static poly static_ideal_generator_619 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 619, permutation);  generators.emplace_front(static_ideal_generator_619); \
	case 618:  static poly static_ideal_generator_618 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 618, permutation);  generators.emplace_front(static_ideal_generator_618); \
	case 617:  static poly static_ideal_generator_617 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 617, permutation);  generators.emplace_front(static_ideal_generator_617); \
	case 616:  static poly static_ideal_generator_616 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 616, permutation);  generators.emplace_front(static_ideal_generator_616); \
	case 615:  static poly static_ideal_generator_615 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 615, permutation);  generators.emplace_front(static_ideal_generator_615); \
	case 614:  static poly static_ideal_generator_614 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 614, permutation);  generators.emplace_front(static_ideal_generator_614); \
	case 613:  static poly static_ideal_generator_613 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 613, permutation);  generators.emplace_front(static_ideal_generator_613); \
	case 612:  static poly static_ideal_generator_612 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 612, permutation);  generators.emplace_front(static_ideal_generator_612); \
	case 611:  static poly static_ideal_generator_611 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 611, permutation);  generators.emplace_front(static_ideal_generator_611); \
	case 610:  static poly static_ideal_generator_610 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 610, permutation);  generators.emplace_front(static_ideal_generator_610); \
	case 609:  static poly static_ideal_generator_609 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 609, permutation);  generators.emplace_front(static_ideal_generator_609); \
	case 608:  static poly static_ideal_generator_608 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 608, permutation);  generators.emplace_front(static_ideal_generator_608); \
	case 607:  static poly static_ideal_generator_607 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 607, permutation);  generators.emplace_front(static_ideal_generator_607); \
	case 606:  static poly static_ideal_generator_606 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 606, permutation);  generators.emplace_front(static_ideal_generator_606); \
	case 605:  static poly static_ideal_generator_605 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 605, permutation);  generators.emplace_front(static_ideal_generator_605); \
	case 604:  static poly static_ideal_generator_604 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 604, permutation);  generators.emplace_front(static_ideal_generator_604); \
	case 603:  static poly static_ideal_generator_603 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 603, permutation);  generators.emplace_front(static_ideal_generator_603); \
	case 602:  static poly static_ideal_generator_602 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 602, permutation);  generators.emplace_front(static_ideal_generator_602); \
	case 601:  static poly static_ideal_generator_601 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 601, permutation);  generators.emplace_front(static_ideal_generator_601); \
	case 600:  static poly static_ideal_generator_600 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 600, permutation);  generators.emplace_front(static_ideal_generator_600); \
	case 599:  static poly static_ideal_generator_599 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 599, permutation);  generators.emplace_front(static_ideal_generator_599); \
	case 598:  static poly static_ideal_generator_598 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 598, permutation);  generators.emplace_front(static_ideal_generator_598); \
	case 597:  static poly static_ideal_generator_597 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 597, permutation);  generators.emplace_front(static_ideal_generator_597); \
	case 596:  static poly static_ideal_generator_596 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 596, permutation);  generators.emplace_front(static_ideal_generator_596); \
	case 595:  static poly static_ideal_generator_595 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 595, permutation);  generators.emplace_front(static_ideal_generator_595); \
	case 594:  static poly static_ideal_generator_594 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 594, permutation);  generators.emplace_front(static_ideal_generator_594); \
	case 593:  static poly static_ideal_generator_593 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 593, permutation);  generators.emplace_front(static_ideal_generator_593); \
	case 592:  static poly static_ideal_generator_592 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 592, permutation);  generators.emplace_front(static_ideal_generator_592); \
	case 591:  static poly static_ideal_generator_591 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 591, permutation);  generators.emplace_front(static_ideal_generator_591); \
	case 590:  static poly static_ideal_generator_590 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 590, permutation);  generators.emplace_front(static_ideal_generator_590); \
	case 589:  static poly static_ideal_generator_589 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 589, permutation);  generators.emplace_front(static_ideal_generator_589); \
	case 588:  static poly static_ideal_generator_588 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 588, permutation);  generators.emplace_front(static_ideal_generator_588); \
	case 587:  static poly static_ideal_generator_587 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 587, permutation);  generators.emplace_front(static_ideal_generator_587); \
	case 586:  static poly static_ideal_generator_586 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 586, permutation);  generators.emplace_front(static_ideal_generator_586); \
	case 585:  static poly static_ideal_generator_585 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 585, permutation);  generators.emplace_front(static_ideal_generator_585); \
	case 584:  static poly static_ideal_generator_584 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 584, permutation);  generators.emplace_front(static_ideal_generator_584); \
	case 583:  static poly static_ideal_generator_583 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 583, permutation);  generators.emplace_front(static_ideal_generator_583); \
	case 582:  static poly static_ideal_generator_582 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 582, permutation);  generators.emplace_front(static_ideal_generator_582); \
	case 581:  static poly static_ideal_generator_581 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 581, permutation);  generators.emplace_front(static_ideal_generator_581); \
	case 580:  static poly static_ideal_generator_580 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 580, permutation);  generators.emplace_front(static_ideal_generator_580); \
	case 579:  static poly static_ideal_generator_579 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 579, permutation);  generators.emplace_front(static_ideal_generator_579); \
	case 578:  static poly static_ideal_generator_578 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 578, permutation);  generators.emplace_front(static_ideal_generator_578); \
	case 577:  static poly static_ideal_generator_577 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 577, permutation);  generators.emplace_front(static_ideal_generator_577); \
	case 576:  static poly static_ideal_generator_576 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 576, permutation);  generators.emplace_front(static_ideal_generator_576); \
	case 575:  static poly static_ideal_generator_575 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 575, permutation);  generators.emplace_front(static_ideal_generator_575); \
	case 574:  static poly static_ideal_generator_574 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 574, permutation);  generators.emplace_front(static_ideal_generator_574); \
	case 573:  static poly static_ideal_generator_573 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 573, permutation);  generators.emplace_front(static_ideal_generator_573); \
	case 572:  static poly static_ideal_generator_572 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 572, permutation);  generators.emplace_front(static_ideal_generator_572); \
	case 571:  static poly static_ideal_generator_571 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 571, permutation);  generators.emplace_front(static_ideal_generator_571); \
	case 570:  static poly static_ideal_generator_570 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 570, permutation);  generators.emplace_front(static_ideal_generator_570); \
	case 569:  static poly static_ideal_generator_569 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 569, permutation);  generators.emplace_front(static_ideal_generator_569); \
	case 568:  static poly static_ideal_generator_568 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 568, permutation);  generators.emplace_front(static_ideal_generator_568); \
	case 567:  static poly static_ideal_generator_567 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 567, permutation);  generators.emplace_front(static_ideal_generator_567); \
	case 566:  static poly static_ideal_generator_566 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 566, permutation);  generators.emplace_front(static_ideal_generator_566); \
	case 565:  static poly static_ideal_generator_565 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 565, permutation);  generators.emplace_front(static_ideal_generator_565); \
	case 564:  static poly static_ideal_generator_564 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 564, permutation);  generators.emplace_front(static_ideal_generator_564); \
	case 563:  static poly static_ideal_generator_563 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 563, permutation);  generators.emplace_front(static_ideal_generator_563); \
	case 562:  static poly static_ideal_generator_562 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 562, permutation);  generators.emplace_front(static_ideal_generator_562); \
	case 561:  static poly static_ideal_generator_561 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 561, permutation);  generators.emplace_front(static_ideal_generator_561); \
	case 560:  static poly static_ideal_generator_560 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 560, permutation);  generators.emplace_front(static_ideal_generator_560); \
	case 559:  static poly static_ideal_generator_559 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 559, permutation);  generators.emplace_front(static_ideal_generator_559); \
	case 558:  static poly static_ideal_generator_558 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 558, permutation);  generators.emplace_front(static_ideal_generator_558); \
	case 557:  static poly static_ideal_generator_557 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 557, permutation);  generators.emplace_front(static_ideal_generator_557); \
	case 556:  static poly static_ideal_generator_556 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 556, permutation);  generators.emplace_front(static_ideal_generator_556); \
	case 555:  static poly static_ideal_generator_555 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 555, permutation);  generators.emplace_front(static_ideal_generator_555); \
	case 554:  static poly static_ideal_generator_554 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 554, permutation);  generators.emplace_front(static_ideal_generator_554); \
	case 553:  static poly static_ideal_generator_553 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 553, permutation);  generators.emplace_front(static_ideal_generator_553); \
	case 552:  static poly static_ideal_generator_552 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 552, permutation);  generators.emplace_front(static_ideal_generator_552); \
	case 551:  static poly static_ideal_generator_551 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 551, permutation);  generators.emplace_front(static_ideal_generator_551); \
	case 550:  static poly static_ideal_generator_550 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 550, permutation);  generators.emplace_front(static_ideal_generator_550); \
	case 549:  static poly static_ideal_generator_549 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 549, permutation);  generators.emplace_front(static_ideal_generator_549); \
	case 548:  static poly static_ideal_generator_548 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 548, permutation);  generators.emplace_front(static_ideal_generator_548); \
	case 547:  static poly static_ideal_generator_547 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 547, permutation);  generators.emplace_front(static_ideal_generator_547); \
	case 546:  static poly static_ideal_generator_546 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 546, permutation);  generators.emplace_front(static_ideal_generator_546); \
	case 545:  static poly static_ideal_generator_545 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 545, permutation);  generators.emplace_front(static_ideal_generator_545); \
	case 544:  static poly static_ideal_generator_544 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 544, permutation);  generators.emplace_front(static_ideal_generator_544); \
	case 543:  static poly static_ideal_generator_543 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 543, permutation);  generators.emplace_front(static_ideal_generator_543); \
	case 542:  static poly static_ideal_generator_542 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 542, permutation);  generators.emplace_front(static_ideal_generator_542); \
	case 541:  static poly static_ideal_generator_541 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 541, permutation);  generators.emplace_front(static_ideal_generator_541); \
	case 540:  static poly static_ideal_generator_540 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 540, permutation);  generators.emplace_front(static_ideal_generator_540); \
	case 539:  static poly static_ideal_generator_539 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 539, permutation);  generators.emplace_front(static_ideal_generator_539); \
	case 538:  static poly static_ideal_generator_538 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 538, permutation);  generators.emplace_front(static_ideal_generator_538); \
	case 537:  static poly static_ideal_generator_537 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 537, permutation);  generators.emplace_front(static_ideal_generator_537); \
	case 536:  static poly static_ideal_generator_536 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 536, permutation);  generators.emplace_front(static_ideal_generator_536); \
	case 535:  static poly static_ideal_generator_535 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 535, permutation);  generators.emplace_front(static_ideal_generator_535); \
	case 534:  static poly static_ideal_generator_534 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 534, permutation);  generators.emplace_front(static_ideal_generator_534); \
	case 533:  static poly static_ideal_generator_533 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 533, permutation);  generators.emplace_front(static_ideal_generator_533); \
	case 532:  static poly static_ideal_generator_532 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 532, permutation);  generators.emplace_front(static_ideal_generator_532); \
	case 531:  static poly static_ideal_generator_531 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 531, permutation);  generators.emplace_front(static_ideal_generator_531); \
	case 530:  static poly static_ideal_generator_530 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 530, permutation);  generators.emplace_front(static_ideal_generator_530); \
	case 529:  static poly static_ideal_generator_529 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 529, permutation);  generators.emplace_front(static_ideal_generator_529); \
	case 528:  static poly static_ideal_generator_528 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 528, permutation);  generators.emplace_front(static_ideal_generator_528); \
	case 527:  static poly static_ideal_generator_527 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 527, permutation);  generators.emplace_front(static_ideal_generator_527); \
	case 526:  static poly static_ideal_generator_526 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 526, permutation);  generators.emplace_front(static_ideal_generator_526); \
	case 525:  static poly static_ideal_generator_525 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 525, permutation);  generators.emplace_front(static_ideal_generator_525); \
	case 524:  static poly static_ideal_generator_524 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 524, permutation);  generators.emplace_front(static_ideal_generator_524); \
	case 523:  static poly static_ideal_generator_523 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 523, permutation);  generators.emplace_front(static_ideal_generator_523); \
	case 522:  static poly static_ideal_generator_522 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 522, permutation);  generators.emplace_front(static_ideal_generator_522); \
	case 521:  static poly static_ideal_generator_521 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 521, permutation);  generators.emplace_front(static_ideal_generator_521); \
	case 520:  static poly static_ideal_generator_520 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 520, permutation);  generators.emplace_front(static_ideal_generator_520); \
	case 519:  static poly static_ideal_generator_519 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 519, permutation);  generators.emplace_front(static_ideal_generator_519); \
	case 518:  static poly static_ideal_generator_518 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 518, permutation);  generators.emplace_front(static_ideal_generator_518); \
	case 517:  static poly static_ideal_generator_517 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 517, permutation);  generators.emplace_front(static_ideal_generator_517); \
	case 516:  static poly static_ideal_generator_516 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 516, permutation);  generators.emplace_front(static_ideal_generator_516); \
	case 515:  static poly static_ideal_generator_515 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 515, permutation);  generators.emplace_front(static_ideal_generator_515); \
	case 514:  static poly static_ideal_generator_514 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 514, permutation);  generators.emplace_front(static_ideal_generator_514); \
	case 513:  static poly static_ideal_generator_513 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 513, permutation);  generators.emplace_front(static_ideal_generator_513); \
	case 512:  static poly static_ideal_generator_512 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 512, permutation);  generators.emplace_front(static_ideal_generator_512); \
	case 511:  static poly static_ideal_generator_511 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 511, permutation);  generators.emplace_front(static_ideal_generator_511); \
	case 510:  static poly static_ideal_generator_510 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 510, permutation);  generators.emplace_front(static_ideal_generator_510); \
	case 509:  static poly static_ideal_generator_509 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 509, permutation);  generators.emplace_front(static_ideal_generator_509); \
	case 508:  static poly static_ideal_generator_508 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 508, permutation);  generators.emplace_front(static_ideal_generator_508); \
	case 507:  static poly static_ideal_generator_507 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 507, permutation);  generators.emplace_front(static_ideal_generator_507); \
	case 506:  static poly static_ideal_generator_506 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 506, permutation);  generators.emplace_front(static_ideal_generator_506); \
	case 505:  static poly static_ideal_generator_505 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 505, permutation);  generators.emplace_front(static_ideal_generator_505); \
	case 504:  static poly static_ideal_generator_504 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 504, permutation);  generators.emplace_front(static_ideal_generator_504); \
	case 503:  static poly static_ideal_generator_503 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 503, permutation);  generators.emplace_front(static_ideal_generator_503); \
	case 502:  static poly static_ideal_generator_502 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 502, permutation);  generators.emplace_front(static_ideal_generator_502); \
	case 501:  static poly static_ideal_generator_501 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 501, permutation);  generators.emplace_front(static_ideal_generator_501); \
	case 500:  static poly static_ideal_generator_500 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 500, permutation);  generators.emplace_front(static_ideal_generator_500); \
	case 499:  static poly static_ideal_generator_499 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 499, permutation);  generators.emplace_front(static_ideal_generator_499); \
	case 498:  static poly static_ideal_generator_498 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 498, permutation);  generators.emplace_front(static_ideal_generator_498); \
	case 497:  static poly static_ideal_generator_497 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 497, permutation);  generators.emplace_front(static_ideal_generator_497); \
	case 496:  static poly static_ideal_generator_496 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 496, permutation);  generators.emplace_front(static_ideal_generator_496); \
	case 495:  static poly static_ideal_generator_495 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 495, permutation);  generators.emplace_front(static_ideal_generator_495); \
	case 494:  static poly static_ideal_generator_494 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 494, permutation);  generators.emplace_front(static_ideal_generator_494); \
	case 493:  static poly static_ideal_generator_493 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 493, permutation);  generators.emplace_front(static_ideal_generator_493); \
	case 492:  static poly static_ideal_generator_492 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 492, permutation);  generators.emplace_front(static_ideal_generator_492); \
	case 491:  static poly static_ideal_generator_491 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 491, permutation);  generators.emplace_front(static_ideal_generator_491); \
	case 490:  static poly static_ideal_generator_490 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 490, permutation);  generators.emplace_front(static_ideal_generator_490); \
	case 489:  static poly static_ideal_generator_489 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 489, permutation);  generators.emplace_front(static_ideal_generator_489); \
	case 488:  static poly static_ideal_generator_488 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 488, permutation);  generators.emplace_front(static_ideal_generator_488); \
	case 487:  static poly static_ideal_generator_487 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 487, permutation);  generators.emplace_front(static_ideal_generator_487); \
	case 486:  static poly static_ideal_generator_486 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 486, permutation);  generators.emplace_front(static_ideal_generator_486); \
	case 485:  static poly static_ideal_generator_485 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 485, permutation);  generators.emplace_front(static_ideal_generator_485); \
	case 484:  static poly static_ideal_generator_484 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 484, permutation);  generators.emplace_front(static_ideal_generator_484); \
	case 483:  static poly static_ideal_generator_483 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 483, permutation);  generators.emplace_front(static_ideal_generator_483); \
	case 482:  static poly static_ideal_generator_482 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 482, permutation);  generators.emplace_front(static_ideal_generator_482); \
	case 481:  static poly static_ideal_generator_481 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 481, permutation);  generators.emplace_front(static_ideal_generator_481); \
	case 480:  static poly static_ideal_generator_480 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 480, permutation);  generators.emplace_front(static_ideal_generator_480); \
	case 479:  static poly static_ideal_generator_479 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 479, permutation);  generators.emplace_front(static_ideal_generator_479); \
	case 478:  static poly static_ideal_generator_478 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 478, permutation);  generators.emplace_front(static_ideal_generator_478); \
	case 477:  static poly static_ideal_generator_477 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 477, permutation);  generators.emplace_front(static_ideal_generator_477); \
	case 476:  static poly static_ideal_generator_476 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 476, permutation);  generators.emplace_front(static_ideal_generator_476); \
	case 475:  static poly static_ideal_generator_475 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 475, permutation);  generators.emplace_front(static_ideal_generator_475); \
	case 474:  static poly static_ideal_generator_474 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 474, permutation);  generators.emplace_front(static_ideal_generator_474); \
	case 473:  static poly static_ideal_generator_473 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 473, permutation);  generators.emplace_front(static_ideal_generator_473); \
	case 472:  static poly static_ideal_generator_472 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 472, permutation);  generators.emplace_front(static_ideal_generator_472); \
	case 471:  static poly static_ideal_generator_471 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 471, permutation);  generators.emplace_front(static_ideal_generator_471); \
	case 470:  static poly static_ideal_generator_470 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 470, permutation);  generators.emplace_front(static_ideal_generator_470); \
	case 469:  static poly static_ideal_generator_469 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 469, permutation);  generators.emplace_front(static_ideal_generator_469); \
	case 468:  static poly static_ideal_generator_468 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 468, permutation);  generators.emplace_front(static_ideal_generator_468); \
	case 467:  static poly static_ideal_generator_467 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 467, permutation);  generators.emplace_front(static_ideal_generator_467); \
	case 466:  static poly static_ideal_generator_466 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 466, permutation);  generators.emplace_front(static_ideal_generator_466); \
	case 465:  static poly static_ideal_generator_465 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 465, permutation);  generators.emplace_front(static_ideal_generator_465); \
	case 464:  static poly static_ideal_generator_464 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 464, permutation);  generators.emplace_front(static_ideal_generator_464); \
	case 463:  static poly static_ideal_generator_463 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 463, permutation);  generators.emplace_front(static_ideal_generator_463); \
	case 462:  static poly static_ideal_generator_462 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 462, permutation);  generators.emplace_front(static_ideal_generator_462); \
	case 461:  static poly static_ideal_generator_461 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 461, permutation);  generators.emplace_front(static_ideal_generator_461); \
	case 460:  static poly static_ideal_generator_460 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 460, permutation);  generators.emplace_front(static_ideal_generator_460); \
	case 459:  static poly static_ideal_generator_459 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 459, permutation);  generators.emplace_front(static_ideal_generator_459); \
	case 458:  static poly static_ideal_generator_458 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 458, permutation);  generators.emplace_front(static_ideal_generator_458); \
	case 457:  static poly static_ideal_generator_457 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 457, permutation);  generators.emplace_front(static_ideal_generator_457); \
	case 456:  static poly static_ideal_generator_456 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 456, permutation);  generators.emplace_front(static_ideal_generator_456); \
	case 455:  static poly static_ideal_generator_455 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 455, permutation);  generators.emplace_front(static_ideal_generator_455); \
	case 454:  static poly static_ideal_generator_454 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 454, permutation);  generators.emplace_front(static_ideal_generator_454); \
	case 453:  static poly static_ideal_generator_453 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 453, permutation);  generators.emplace_front(static_ideal_generator_453); \
	case 452:  static poly static_ideal_generator_452 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 452, permutation);  generators.emplace_front(static_ideal_generator_452); \
	case 451:  static poly static_ideal_generator_451 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 451, permutation);  generators.emplace_front(static_ideal_generator_451); \
	case 450:  static poly static_ideal_generator_450 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 450, permutation);  generators.emplace_front(static_ideal_generator_450); \
	case 449:  static poly static_ideal_generator_449 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 449, permutation);  generators.emplace_front(static_ideal_generator_449); \
	case 448:  static poly static_ideal_generator_448 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 448, permutation);  generators.emplace_front(static_ideal_generator_448); \
	case 447:  static poly static_ideal_generator_447 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 447, permutation);  generators.emplace_front(static_ideal_generator_447); \
	case 446:  static poly static_ideal_generator_446 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 446, permutation);  generators.emplace_front(static_ideal_generator_446); \
	case 445:  static poly static_ideal_generator_445 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 445, permutation);  generators.emplace_front(static_ideal_generator_445); \
	case 444:  static poly static_ideal_generator_444 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 444, permutation);  generators.emplace_front(static_ideal_generator_444); \
	case 443:  static poly static_ideal_generator_443 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 443, permutation);  generators.emplace_front(static_ideal_generator_443); \
	case 442:  static poly static_ideal_generator_442 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 442, permutation);  generators.emplace_front(static_ideal_generator_442); \
	case 441:  static poly static_ideal_generator_441 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 441, permutation);  generators.emplace_front(static_ideal_generator_441); \
	case 440:  static poly static_ideal_generator_440 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 440, permutation);  generators.emplace_front(static_ideal_generator_440); \
	case 439:  static poly static_ideal_generator_439 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 439, permutation);  generators.emplace_front(static_ideal_generator_439); \
	case 438:  static poly static_ideal_generator_438 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 438, permutation);  generators.emplace_front(static_ideal_generator_438); \
	case 437:  static poly static_ideal_generator_437 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 437, permutation);  generators.emplace_front(static_ideal_generator_437); \
	case 436:  static poly static_ideal_generator_436 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 436, permutation);  generators.emplace_front(static_ideal_generator_436); \
	case 435:  static poly static_ideal_generator_435 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 435, permutation);  generators.emplace_front(static_ideal_generator_435); \
	case 434:  static poly static_ideal_generator_434 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 434, permutation);  generators.emplace_front(static_ideal_generator_434); \
	case 433:  static poly static_ideal_generator_433 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 433, permutation);  generators.emplace_front(static_ideal_generator_433); \
	case 432:  static poly static_ideal_generator_432 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 432, permutation);  generators.emplace_front(static_ideal_generator_432); \
	case 431:  static poly static_ideal_generator_431 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 431, permutation);  generators.emplace_front(static_ideal_generator_431); \
	case 430:  static poly static_ideal_generator_430 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 430, permutation);  generators.emplace_front(static_ideal_generator_430); \
	case 429:  static poly static_ideal_generator_429 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 429, permutation);  generators.emplace_front(static_ideal_generator_429); \
	case 428:  static poly static_ideal_generator_428 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 428, permutation);  generators.emplace_front(static_ideal_generator_428); \
	case 427:  static poly static_ideal_generator_427 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 427, permutation);  generators.emplace_front(static_ideal_generator_427); \
	case 426:  static poly static_ideal_generator_426 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 426, permutation);  generators.emplace_front(static_ideal_generator_426); \
	case 425:  static poly static_ideal_generator_425 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 425, permutation);  generators.emplace_front(static_ideal_generator_425); \
	case 424:  static poly static_ideal_generator_424 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 424, permutation);  generators.emplace_front(static_ideal_generator_424); \
	case 423:  static poly static_ideal_generator_423 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 423, permutation);  generators.emplace_front(static_ideal_generator_423); \
	case 422:  static poly static_ideal_generator_422 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 422, permutation);  generators.emplace_front(static_ideal_generator_422); \
	case 421:  static poly static_ideal_generator_421 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 421, permutation);  generators.emplace_front(static_ideal_generator_421); \
	case 420:  static poly static_ideal_generator_420 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 420, permutation);  generators.emplace_front(static_ideal_generator_420); \
	case 419:  static poly static_ideal_generator_419 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 419, permutation);  generators.emplace_front(static_ideal_generator_419); \
	case 418:  static poly static_ideal_generator_418 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 418, permutation);  generators.emplace_front(static_ideal_generator_418); \
	case 417:  static poly static_ideal_generator_417 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 417, permutation);  generators.emplace_front(static_ideal_generator_417); \
	case 416:  static poly static_ideal_generator_416 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 416, permutation);  generators.emplace_front(static_ideal_generator_416); \
	case 415:  static poly static_ideal_generator_415 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 415, permutation);  generators.emplace_front(static_ideal_generator_415); \
	case 414:  static poly static_ideal_generator_414 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 414, permutation);  generators.emplace_front(static_ideal_generator_414); \
	case 413:  static poly static_ideal_generator_413 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 413, permutation);  generators.emplace_front(static_ideal_generator_413); \
	case 412:  static poly static_ideal_generator_412 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 412, permutation);  generators.emplace_front(static_ideal_generator_412); \
	case 411:  static poly static_ideal_generator_411 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 411, permutation);  generators.emplace_front(static_ideal_generator_411); \
	case 410:  static poly static_ideal_generator_410 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 410, permutation);  generators.emplace_front(static_ideal_generator_410); \
	case 409:  static poly static_ideal_generator_409 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 409, permutation);  generators.emplace_front(static_ideal_generator_409); \
	case 408:  static poly static_ideal_generator_408 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 408, permutation);  generators.emplace_front(static_ideal_generator_408); \
	case 407:  static poly static_ideal_generator_407 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 407, permutation);  generators.emplace_front(static_ideal_generator_407); \
	case 406:  static poly static_ideal_generator_406 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 406, permutation);  generators.emplace_front(static_ideal_generator_406); \
	case 405:  static poly static_ideal_generator_405 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 405, permutation);  generators.emplace_front(static_ideal_generator_405); \
	case 404:  static poly static_ideal_generator_404 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 404, permutation);  generators.emplace_front(static_ideal_generator_404); \
	case 403:  static poly static_ideal_generator_403 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 403, permutation);  generators.emplace_front(static_ideal_generator_403); \
	case 402:  static poly static_ideal_generator_402 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 402, permutation);  generators.emplace_front(static_ideal_generator_402); \
	case 401:  static poly static_ideal_generator_401 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 401, permutation);  generators.emplace_front(static_ideal_generator_401); \
	case 400:  static poly static_ideal_generator_400 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 400, permutation);  generators.emplace_front(static_ideal_generator_400); \
	case 399:  static poly static_ideal_generator_399 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 399, permutation);  generators.emplace_front(static_ideal_generator_399); \
	case 398:  static poly static_ideal_generator_398 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 398, permutation);  generators.emplace_front(static_ideal_generator_398); \
	case 397:  static poly static_ideal_generator_397 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 397, permutation);  generators.emplace_front(static_ideal_generator_397); \
	case 396:  static poly static_ideal_generator_396 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 396, permutation);  generators.emplace_front(static_ideal_generator_396); \
	case 395:  static poly static_ideal_generator_395 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 395, permutation);  generators.emplace_front(static_ideal_generator_395); \
	case 394:  static poly static_ideal_generator_394 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 394, permutation);  generators.emplace_front(static_ideal_generator_394); \
	case 393:  static poly static_ideal_generator_393 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 393, permutation);  generators.emplace_front(static_ideal_generator_393); \
	case 392:  static poly static_ideal_generator_392 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 392, permutation);  generators.emplace_front(static_ideal_generator_392); \
	case 391:  static poly static_ideal_generator_391 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 391, permutation);  generators.emplace_front(static_ideal_generator_391); \
	case 390:  static poly static_ideal_generator_390 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 390, permutation);  generators.emplace_front(static_ideal_generator_390); \
	case 389:  static poly static_ideal_generator_389 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 389, permutation);  generators.emplace_front(static_ideal_generator_389); \
	case 388:  static poly static_ideal_generator_388 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 388, permutation);  generators.emplace_front(static_ideal_generator_388); \
	case 387:  static poly static_ideal_generator_387 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 387, permutation);  generators.emplace_front(static_ideal_generator_387); \
	case 386:  static poly static_ideal_generator_386 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 386, permutation);  generators.emplace_front(static_ideal_generator_386); \
	case 385:  static poly static_ideal_generator_385 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 385, permutation);  generators.emplace_front(static_ideal_generator_385); \
	case 384:  static poly static_ideal_generator_384 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 384, permutation);  generators.emplace_front(static_ideal_generator_384); \
	case 383:  static poly static_ideal_generator_383 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 383, permutation);  generators.emplace_front(static_ideal_generator_383); \
	case 382:  static poly static_ideal_generator_382 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 382, permutation);  generators.emplace_front(static_ideal_generator_382); \
	case 381:  static poly static_ideal_generator_381 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 381, permutation);  generators.emplace_front(static_ideal_generator_381); \
	case 380:  static poly static_ideal_generator_380 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 380, permutation);  generators.emplace_front(static_ideal_generator_380); \
	case 379:  static poly static_ideal_generator_379 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 379, permutation);  generators.emplace_front(static_ideal_generator_379); \
	case 378:  static poly static_ideal_generator_378 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 378, permutation);  generators.emplace_front(static_ideal_generator_378); \
	case 377:  static poly static_ideal_generator_377 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 377, permutation);  generators.emplace_front(static_ideal_generator_377); \
	case 376:  static poly static_ideal_generator_376 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 376, permutation);  generators.emplace_front(static_ideal_generator_376); \
	case 375:  static poly static_ideal_generator_375 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 375, permutation);  generators.emplace_front(static_ideal_generator_375); \
	case 374:  static poly static_ideal_generator_374 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 374, permutation);  generators.emplace_front(static_ideal_generator_374); \
	case 373:  static poly static_ideal_generator_373 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 373, permutation);  generators.emplace_front(static_ideal_generator_373); \
	case 372:  static poly static_ideal_generator_372 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 372, permutation);  generators.emplace_front(static_ideal_generator_372); \
	case 371:  static poly static_ideal_generator_371 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 371, permutation);  generators.emplace_front(static_ideal_generator_371); \
	case 370:  static poly static_ideal_generator_370 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 370, permutation);  generators.emplace_front(static_ideal_generator_370); \
	case 369:  static poly static_ideal_generator_369 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 369, permutation);  generators.emplace_front(static_ideal_generator_369); \
	case 368:  static poly static_ideal_generator_368 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 368, permutation);  generators.emplace_front(static_ideal_generator_368); \
	case 367:  static poly static_ideal_generator_367 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 367, permutation);  generators.emplace_front(static_ideal_generator_367); \
	case 366:  static poly static_ideal_generator_366 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 366, permutation);  generators.emplace_front(static_ideal_generator_366); \
	case 365:  static poly static_ideal_generator_365 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 365, permutation);  generators.emplace_front(static_ideal_generator_365); \
	case 364:  static poly static_ideal_generator_364 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 364, permutation);  generators.emplace_front(static_ideal_generator_364); \
	case 363:  static poly static_ideal_generator_363 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 363, permutation);  generators.emplace_front(static_ideal_generator_363); \
	case 362:  static poly static_ideal_generator_362 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 362, permutation);  generators.emplace_front(static_ideal_generator_362); \
	case 361:  static poly static_ideal_generator_361 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 361, permutation);  generators.emplace_front(static_ideal_generator_361); \
	case 360:  static poly static_ideal_generator_360 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 360, permutation);  generators.emplace_front(static_ideal_generator_360); \
	case 359:  static poly static_ideal_generator_359 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 359, permutation);  generators.emplace_front(static_ideal_generator_359); \
	case 358:  static poly static_ideal_generator_358 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 358, permutation);  generators.emplace_front(static_ideal_generator_358); \
	case 357:  static poly static_ideal_generator_357 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 357, permutation);  generators.emplace_front(static_ideal_generator_357); \
	case 356:  static poly static_ideal_generator_356 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 356, permutation);  generators.emplace_front(static_ideal_generator_356); \
	case 355:  static poly static_ideal_generator_355 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 355, permutation);  generators.emplace_front(static_ideal_generator_355); \
	case 354:  static poly static_ideal_generator_354 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 354, permutation);  generators.emplace_front(static_ideal_generator_354); \
	case 353:  static poly static_ideal_generator_353 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 353, permutation);  generators.emplace_front(static_ideal_generator_353); \
	case 352:  static poly static_ideal_generator_352 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 352, permutation);  generators.emplace_front(static_ideal_generator_352); \
	case 351:  static poly static_ideal_generator_351 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 351, permutation);  generators.emplace_front(static_ideal_generator_351); \
	case 350:  static poly static_ideal_generator_350 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 350, permutation);  generators.emplace_front(static_ideal_generator_350); \
	case 349:  static poly static_ideal_generator_349 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 349, permutation);  generators.emplace_front(static_ideal_generator_349); \
	case 348:  static poly static_ideal_generator_348 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 348, permutation);  generators.emplace_front(static_ideal_generator_348); \
	case 347:  static poly static_ideal_generator_347 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 347, permutation);  generators.emplace_front(static_ideal_generator_347); \
	case 346:  static poly static_ideal_generator_346 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 346, permutation);  generators.emplace_front(static_ideal_generator_346); \
	case 345:  static poly static_ideal_generator_345 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 345, permutation);  generators.emplace_front(static_ideal_generator_345); \
	case 344:  static poly static_ideal_generator_344 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 344, permutation);  generators.emplace_front(static_ideal_generator_344); \
	case 343:  static poly static_ideal_generator_343 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 343, permutation);  generators.emplace_front(static_ideal_generator_343); \
	case 342:  static poly static_ideal_generator_342 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 342, permutation);  generators.emplace_front(static_ideal_generator_342); \
	case 341:  static poly static_ideal_generator_341 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 341, permutation);  generators.emplace_front(static_ideal_generator_341); \
	case 340:  static poly static_ideal_generator_340 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 340, permutation);  generators.emplace_front(static_ideal_generator_340); \
	case 339:  static poly static_ideal_generator_339 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 339, permutation);  generators.emplace_front(static_ideal_generator_339); \
	case 338:  static poly static_ideal_generator_338 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 338, permutation);  generators.emplace_front(static_ideal_generator_338); \
	case 337:  static poly static_ideal_generator_337 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 337, permutation);  generators.emplace_front(static_ideal_generator_337); \
	case 336:  static poly static_ideal_generator_336 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 336, permutation);  generators.emplace_front(static_ideal_generator_336); \
	case 335:  static poly static_ideal_generator_335 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 335, permutation);  generators.emplace_front(static_ideal_generator_335); \
	case 334:  static poly static_ideal_generator_334 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 334, permutation);  generators.emplace_front(static_ideal_generator_334); \
	case 333:  static poly static_ideal_generator_333 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 333, permutation);  generators.emplace_front(static_ideal_generator_333); \
	case 332:  static poly static_ideal_generator_332 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 332, permutation);  generators.emplace_front(static_ideal_generator_332); \
	case 331:  static poly static_ideal_generator_331 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 331, permutation);  generators.emplace_front(static_ideal_generator_331); \
	case 330:  static poly static_ideal_generator_330 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 330, permutation);  generators.emplace_front(static_ideal_generator_330); \
	case 329:  static poly static_ideal_generator_329 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 329, permutation);  generators.emplace_front(static_ideal_generator_329); \
	case 328:  static poly static_ideal_generator_328 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 328, permutation);  generators.emplace_front(static_ideal_generator_328); \
	case 327:  static poly static_ideal_generator_327 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 327, permutation);  generators.emplace_front(static_ideal_generator_327); \
	case 326:  static poly static_ideal_generator_326 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 326, permutation);  generators.emplace_front(static_ideal_generator_326); \
	case 325:  static poly static_ideal_generator_325 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 325, permutation);  generators.emplace_front(static_ideal_generator_325); \
	case 324:  static poly static_ideal_generator_324 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 324, permutation);  generators.emplace_front(static_ideal_generator_324); \
	case 323:  static poly static_ideal_generator_323 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 323, permutation);  generators.emplace_front(static_ideal_generator_323); \
	case 322:  static poly static_ideal_generator_322 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 322, permutation);  generators.emplace_front(static_ideal_generator_322); \
	case 321:  static poly static_ideal_generator_321 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 321, permutation);  generators.emplace_front(static_ideal_generator_321); \
	case 320:  static poly static_ideal_generator_320 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 320, permutation);  generators.emplace_front(static_ideal_generator_320); \
	case 319:  static poly static_ideal_generator_319 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 319, permutation);  generators.emplace_front(static_ideal_generator_319); \
	case 318:  static poly static_ideal_generator_318 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 318, permutation);  generators.emplace_front(static_ideal_generator_318); \
	case 317:  static poly static_ideal_generator_317 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 317, permutation);  generators.emplace_front(static_ideal_generator_317); \
	case 316:  static poly static_ideal_generator_316 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 316, permutation);  generators.emplace_front(static_ideal_generator_316); \
	case 315:  static poly static_ideal_generator_315 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 315, permutation);  generators.emplace_front(static_ideal_generator_315); \
	case 314:  static poly static_ideal_generator_314 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 314, permutation);  generators.emplace_front(static_ideal_generator_314); \
	case 313:  static poly static_ideal_generator_313 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 313, permutation);  generators.emplace_front(static_ideal_generator_313); \
	case 312:  static poly static_ideal_generator_312 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 312, permutation);  generators.emplace_front(static_ideal_generator_312); \
	case 311:  static poly static_ideal_generator_311 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 311, permutation);  generators.emplace_front(static_ideal_generator_311); \
	case 310:  static poly static_ideal_generator_310 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 310, permutation);  generators.emplace_front(static_ideal_generator_310); \
	case 309:  static poly static_ideal_generator_309 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 309, permutation);  generators.emplace_front(static_ideal_generator_309); \
	case 308:  static poly static_ideal_generator_308 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 308, permutation);  generators.emplace_front(static_ideal_generator_308); \
	case 307:  static poly static_ideal_generator_307 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 307, permutation);  generators.emplace_front(static_ideal_generator_307); \
	case 306:  static poly static_ideal_generator_306 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 306, permutation);  generators.emplace_front(static_ideal_generator_306); \
	case 305:  static poly static_ideal_generator_305 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 305, permutation);  generators.emplace_front(static_ideal_generator_305); \
	case 304:  static poly static_ideal_generator_304 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 304, permutation);  generators.emplace_front(static_ideal_generator_304); \
	case 303:  static poly static_ideal_generator_303 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 303, permutation);  generators.emplace_front(static_ideal_generator_303); \
	case 302:  static poly static_ideal_generator_302 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 302, permutation);  generators.emplace_front(static_ideal_generator_302); \
	case 301:  static poly static_ideal_generator_301 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 301, permutation);  generators.emplace_front(static_ideal_generator_301); \
	case 300:  static poly static_ideal_generator_300 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 300, permutation);  generators.emplace_front(static_ideal_generator_300); \
	case 299:  static poly static_ideal_generator_299 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 299, permutation);  generators.emplace_front(static_ideal_generator_299); \
	case 298:  static poly static_ideal_generator_298 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 298, permutation);  generators.emplace_front(static_ideal_generator_298); \
	case 297:  static poly static_ideal_generator_297 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 297, permutation);  generators.emplace_front(static_ideal_generator_297); \
	case 296:  static poly static_ideal_generator_296 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 296, permutation);  generators.emplace_front(static_ideal_generator_296); \
	case 295:  static poly static_ideal_generator_295 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 295, permutation);  generators.emplace_front(static_ideal_generator_295); \
	case 294:  static poly static_ideal_generator_294 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 294, permutation);  generators.emplace_front(static_ideal_generator_294); \
	case 293:  static poly static_ideal_generator_293 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 293, permutation);  generators.emplace_front(static_ideal_generator_293); \
	case 292:  static poly static_ideal_generator_292 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 292, permutation);  generators.emplace_front(static_ideal_generator_292); \
	case 291:  static poly static_ideal_generator_291 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 291, permutation);  generators.emplace_front(static_ideal_generator_291); \
	case 290:  static poly static_ideal_generator_290 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 290, permutation);  generators.emplace_front(static_ideal_generator_290); \
	case 289:  static poly static_ideal_generator_289 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 289, permutation);  generators.emplace_front(static_ideal_generator_289); \
	case 288:  static poly static_ideal_generator_288 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 288, permutation);  generators.emplace_front(static_ideal_generator_288); \
	case 287:  static poly static_ideal_generator_287 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 287, permutation);  generators.emplace_front(static_ideal_generator_287); \
	case 286:  static poly static_ideal_generator_286 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 286, permutation);  generators.emplace_front(static_ideal_generator_286); \
	case 285:  static poly static_ideal_generator_285 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 285, permutation);  generators.emplace_front(static_ideal_generator_285); \
	case 284:  static poly static_ideal_generator_284 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 284, permutation);  generators.emplace_front(static_ideal_generator_284); \
	case 283:  static poly static_ideal_generator_283 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 283, permutation);  generators.emplace_front(static_ideal_generator_283); \
	case 282:  static poly static_ideal_generator_282 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 282, permutation);  generators.emplace_front(static_ideal_generator_282); \
	case 281:  static poly static_ideal_generator_281 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 281, permutation);  generators.emplace_front(static_ideal_generator_281); \
	case 280:  static poly static_ideal_generator_280 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 280, permutation);  generators.emplace_front(static_ideal_generator_280); \
	case 279:  static poly static_ideal_generator_279 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 279, permutation);  generators.emplace_front(static_ideal_generator_279); \
	case 278:  static poly static_ideal_generator_278 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 278, permutation);  generators.emplace_front(static_ideal_generator_278); \
	case 277:  static poly static_ideal_generator_277 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 277, permutation);  generators.emplace_front(static_ideal_generator_277); \
	case 276:  static poly static_ideal_generator_276 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 276, permutation);  generators.emplace_front(static_ideal_generator_276); \
	case 275:  static poly static_ideal_generator_275 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 275, permutation);  generators.emplace_front(static_ideal_generator_275); \
	case 274:  static poly static_ideal_generator_274 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 274, permutation);  generators.emplace_front(static_ideal_generator_274); \
	case 273:  static poly static_ideal_generator_273 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 273, permutation);  generators.emplace_front(static_ideal_generator_273); \
	case 272:  static poly static_ideal_generator_272 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 272, permutation);  generators.emplace_front(static_ideal_generator_272); \
	case 271:  static poly static_ideal_generator_271 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 271, permutation);  generators.emplace_front(static_ideal_generator_271); \
	case 270:  static poly static_ideal_generator_270 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 270, permutation);  generators.emplace_front(static_ideal_generator_270); \
	case 269:  static poly static_ideal_generator_269 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 269, permutation);  generators.emplace_front(static_ideal_generator_269); \
	case 268:  static poly static_ideal_generator_268 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 268, permutation);  generators.emplace_front(static_ideal_generator_268); \
	case 267:  static poly static_ideal_generator_267 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 267, permutation);  generators.emplace_front(static_ideal_generator_267); \
	case 266:  static poly static_ideal_generator_266 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 266, permutation);  generators.emplace_front(static_ideal_generator_266); \
	case 265:  static poly static_ideal_generator_265 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 265, permutation);  generators.emplace_front(static_ideal_generator_265); \
	case 264:  static poly static_ideal_generator_264 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 264, permutation);  generators.emplace_front(static_ideal_generator_264); \
	case 263:  static poly static_ideal_generator_263 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 263, permutation);  generators.emplace_front(static_ideal_generator_263); \
	case 262:  static poly static_ideal_generator_262 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 262, permutation);  generators.emplace_front(static_ideal_generator_262); \
	case 261:  static poly static_ideal_generator_261 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 261, permutation);  generators.emplace_front(static_ideal_generator_261); \
	case 260:  static poly static_ideal_generator_260 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 260, permutation);  generators.emplace_front(static_ideal_generator_260); \
	case 259:  static poly static_ideal_generator_259 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 259, permutation);  generators.emplace_front(static_ideal_generator_259); \
	case 258:  static poly static_ideal_generator_258 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 258, permutation);  generators.emplace_front(static_ideal_generator_258); \
	case 257:  static poly static_ideal_generator_257 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 257, permutation);  generators.emplace_front(static_ideal_generator_257); \
	case 256:  static poly static_ideal_generator_256 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 256, permutation);  generators.emplace_front(static_ideal_generator_256); \
	case 255:  static poly static_ideal_generator_255 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 255, permutation);  generators.emplace_front(static_ideal_generator_255); \
	case 254:  static poly static_ideal_generator_254 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 254, permutation);  generators.emplace_front(static_ideal_generator_254); \
	case 253:  static poly static_ideal_generator_253 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 253, permutation);  generators.emplace_front(static_ideal_generator_253); \
	case 252:  static poly static_ideal_generator_252 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 252, permutation);  generators.emplace_front(static_ideal_generator_252); \
	case 251:  static poly static_ideal_generator_251 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 251, permutation);  generators.emplace_front(static_ideal_generator_251); \
	case 250:  static poly static_ideal_generator_250 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 250, permutation);  generators.emplace_front(static_ideal_generator_250); \
	case 249:  static poly static_ideal_generator_249 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 249, permutation);  generators.emplace_front(static_ideal_generator_249); \
	case 248:  static poly static_ideal_generator_248 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 248, permutation);  generators.emplace_front(static_ideal_generator_248); \
	case 247:  static poly static_ideal_generator_247 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 247, permutation);  generators.emplace_front(static_ideal_generator_247); \
	case 246:  static poly static_ideal_generator_246 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 246, permutation);  generators.emplace_front(static_ideal_generator_246); \
	case 245:  static poly static_ideal_generator_245 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 245, permutation);  generators.emplace_front(static_ideal_generator_245); \
	case 244:  static poly static_ideal_generator_244 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 244, permutation);  generators.emplace_front(static_ideal_generator_244); \
	case 243:  static poly static_ideal_generator_243 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 243, permutation);  generators.emplace_front(static_ideal_generator_243); \
	case 242:  static poly static_ideal_generator_242 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 242, permutation);  generators.emplace_front(static_ideal_generator_242); \
	case 241:  static poly static_ideal_generator_241 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 241, permutation);  generators.emplace_front(static_ideal_generator_241); \
	case 240:  static poly static_ideal_generator_240 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 240, permutation);  generators.emplace_front(static_ideal_generator_240); \
	case 239:  static poly static_ideal_generator_239 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 239, permutation);  generators.emplace_front(static_ideal_generator_239); \
	case 238:  static poly static_ideal_generator_238 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 238, permutation);  generators.emplace_front(static_ideal_generator_238); \
	case 237:  static poly static_ideal_generator_237 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 237, permutation);  generators.emplace_front(static_ideal_generator_237); \
	case 236:  static poly static_ideal_generator_236 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 236, permutation);  generators.emplace_front(static_ideal_generator_236); \
	case 235:  static poly static_ideal_generator_235 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 235, permutation);  generators.emplace_front(static_ideal_generator_235); \
	case 234:  static poly static_ideal_generator_234 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 234, permutation);  generators.emplace_front(static_ideal_generator_234); \
	case 233:  static poly static_ideal_generator_233 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 233, permutation);  generators.emplace_front(static_ideal_generator_233); \
	case 232:  static poly static_ideal_generator_232 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 232, permutation);  generators.emplace_front(static_ideal_generator_232); \
	case 231:  static poly static_ideal_generator_231 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 231, permutation);  generators.emplace_front(static_ideal_generator_231); \
	case 230:  static poly static_ideal_generator_230 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 230, permutation);  generators.emplace_front(static_ideal_generator_230); \
	case 229:  static poly static_ideal_generator_229 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 229, permutation);  generators.emplace_front(static_ideal_generator_229); \
	case 228:  static poly static_ideal_generator_228 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 228, permutation);  generators.emplace_front(static_ideal_generator_228); \
	case 227:  static poly static_ideal_generator_227 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 227, permutation);  generators.emplace_front(static_ideal_generator_227); \
	case 226:  static poly static_ideal_generator_226 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 226, permutation);  generators.emplace_front(static_ideal_generator_226); \
	case 225:  static poly static_ideal_generator_225 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 225, permutation);  generators.emplace_front(static_ideal_generator_225); \
	case 224:  static poly static_ideal_generator_224 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 224, permutation);  generators.emplace_front(static_ideal_generator_224); \
	case 223:  static poly static_ideal_generator_223 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 223, permutation);  generators.emplace_front(static_ideal_generator_223); \
	case 222:  static poly static_ideal_generator_222 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 222, permutation);  generators.emplace_front(static_ideal_generator_222); \
	case 221:  static poly static_ideal_generator_221 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 221, permutation);  generators.emplace_front(static_ideal_generator_221); \
	case 220:  static poly static_ideal_generator_220 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 220, permutation);  generators.emplace_front(static_ideal_generator_220); \
	case 219:  static poly static_ideal_generator_219 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 219, permutation);  generators.emplace_front(static_ideal_generator_219); \
	case 218:  static poly static_ideal_generator_218 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 218, permutation);  generators.emplace_front(static_ideal_generator_218); \
	case 217:  static poly static_ideal_generator_217 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 217, permutation);  generators.emplace_front(static_ideal_generator_217); \
	case 216:  static poly static_ideal_generator_216 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 216, permutation);  generators.emplace_front(static_ideal_generator_216); \
	case 215:  static poly static_ideal_generator_215 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 215, permutation);  generators.emplace_front(static_ideal_generator_215); \
	case 214:  static poly static_ideal_generator_214 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 214, permutation);  generators.emplace_front(static_ideal_generator_214); \
	case 213:  static poly static_ideal_generator_213 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 213, permutation);  generators.emplace_front(static_ideal_generator_213); \
	case 212:  static poly static_ideal_generator_212 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 212, permutation);  generators.emplace_front(static_ideal_generator_212); \
	case 211:  static poly static_ideal_generator_211 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 211, permutation);  generators.emplace_front(static_ideal_generator_211); \
	case 210:  static poly static_ideal_generator_210 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 210, permutation);  generators.emplace_front(static_ideal_generator_210); \
	case 209:  static poly static_ideal_generator_209 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 209, permutation);  generators.emplace_front(static_ideal_generator_209); \
	case 208:  static poly static_ideal_generator_208 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 208, permutation);  generators.emplace_front(static_ideal_generator_208); \
	case 207:  static poly static_ideal_generator_207 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 207, permutation);  generators.emplace_front(static_ideal_generator_207); \
	case 206:  static poly static_ideal_generator_206 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 206, permutation);  generators.emplace_front(static_ideal_generator_206); \
	case 205:  static poly static_ideal_generator_205 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 205, permutation);  generators.emplace_front(static_ideal_generator_205); \
	case 204:  static poly static_ideal_generator_204 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 204, permutation);  generators.emplace_front(static_ideal_generator_204); \
	case 203:  static poly static_ideal_generator_203 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 203, permutation);  generators.emplace_front(static_ideal_generator_203); \
	case 202:  static poly static_ideal_generator_202 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 202, permutation);  generators.emplace_front(static_ideal_generator_202); \
	case 201:  static poly static_ideal_generator_201 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 201, permutation);  generators.emplace_front(static_ideal_generator_201); \
	case 200:  static poly static_ideal_generator_200 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 200, permutation);  generators.emplace_front(static_ideal_generator_200); \
	case 199:  static poly static_ideal_generator_199 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 199, permutation);  generators.emplace_front(static_ideal_generator_199); \
	case 198:  static poly static_ideal_generator_198 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 198, permutation);  generators.emplace_front(static_ideal_generator_198); \
	case 197:  static poly static_ideal_generator_197 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 197, permutation);  generators.emplace_front(static_ideal_generator_197); \
	case 196:  static poly static_ideal_generator_196 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 196, permutation);  generators.emplace_front(static_ideal_generator_196); \
	case 195:  static poly static_ideal_generator_195 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 195, permutation);  generators.emplace_front(static_ideal_generator_195); \
	case 194:  static poly static_ideal_generator_194 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 194, permutation);  generators.emplace_front(static_ideal_generator_194); \
	case 193:  static poly static_ideal_generator_193 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 193, permutation);  generators.emplace_front(static_ideal_generator_193); \
	case 192:  static poly static_ideal_generator_192 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 192, permutation);  generators.emplace_front(static_ideal_generator_192); \
	case 191:  static poly static_ideal_generator_191 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 191, permutation);  generators.emplace_front(static_ideal_generator_191); \
	case 190:  static poly static_ideal_generator_190 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 190, permutation);  generators.emplace_front(static_ideal_generator_190); \
	case 189:  static poly static_ideal_generator_189 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 189, permutation);  generators.emplace_front(static_ideal_generator_189); \
	case 188:  static poly static_ideal_generator_188 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 188, permutation);  generators.emplace_front(static_ideal_generator_188); \
	case 187:  static poly static_ideal_generator_187 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 187, permutation);  generators.emplace_front(static_ideal_generator_187); \
	case 186:  static poly static_ideal_generator_186 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 186, permutation);  generators.emplace_front(static_ideal_generator_186); \
	case 185:  static poly static_ideal_generator_185 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 185, permutation);  generators.emplace_front(static_ideal_generator_185); \
	case 184:  static poly static_ideal_generator_184 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 184, permutation);  generators.emplace_front(static_ideal_generator_184); \
	case 183:  static poly static_ideal_generator_183 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 183, permutation);  generators.emplace_front(static_ideal_generator_183); \
	case 182:  static poly static_ideal_generator_182 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 182, permutation);  generators.emplace_front(static_ideal_generator_182); \
	case 181:  static poly static_ideal_generator_181 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 181, permutation);  generators.emplace_front(static_ideal_generator_181); \
	case 180:  static poly static_ideal_generator_180 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 180, permutation);  generators.emplace_front(static_ideal_generator_180); \
	case 179:  static poly static_ideal_generator_179 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 179, permutation);  generators.emplace_front(static_ideal_generator_179); \
	case 178:  static poly static_ideal_generator_178 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 178, permutation);  generators.emplace_front(static_ideal_generator_178); \
	case 177:  static poly static_ideal_generator_177 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 177, permutation);  generators.emplace_front(static_ideal_generator_177); \
	case 176:  static poly static_ideal_generator_176 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 176, permutation);  generators.emplace_front(static_ideal_generator_176); \
	case 175:  static poly static_ideal_generator_175 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 175, permutation);  generators.emplace_front(static_ideal_generator_175); \
	case 174:  static poly static_ideal_generator_174 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 174, permutation);  generators.emplace_front(static_ideal_generator_174); \
	case 173:  static poly static_ideal_generator_173 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 173, permutation);  generators.emplace_front(static_ideal_generator_173); \
	case 172:  static poly static_ideal_generator_172 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 172, permutation);  generators.emplace_front(static_ideal_generator_172); \
	case 171:  static poly static_ideal_generator_171 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 171, permutation);  generators.emplace_front(static_ideal_generator_171); \
	case 170:  static poly static_ideal_generator_170 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 170, permutation);  generators.emplace_front(static_ideal_generator_170); \
	case 169:  static poly static_ideal_generator_169 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 169, permutation);  generators.emplace_front(static_ideal_generator_169); \
	case 168:  static poly static_ideal_generator_168 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 168, permutation);  generators.emplace_front(static_ideal_generator_168); \
	case 167:  static poly static_ideal_generator_167 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 167, permutation);  generators.emplace_front(static_ideal_generator_167); \
	case 166:  static poly static_ideal_generator_166 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 166, permutation);  generators.emplace_front(static_ideal_generator_166); \
	case 165:  static poly static_ideal_generator_165 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 165, permutation);  generators.emplace_front(static_ideal_generator_165); \
	case 164:  static poly static_ideal_generator_164 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 164, permutation);  generators.emplace_front(static_ideal_generator_164); \
	case 163:  static poly static_ideal_generator_163 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 163, permutation);  generators.emplace_front(static_ideal_generator_163); \
	case 162:  static poly static_ideal_generator_162 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 162, permutation);  generators.emplace_front(static_ideal_generator_162); \
	case 161:  static poly static_ideal_generator_161 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 161, permutation);  generators.emplace_front(static_ideal_generator_161); \
	case 160:  static poly static_ideal_generator_160 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 160, permutation);  generators.emplace_front(static_ideal_generator_160); \
	case 159:  static poly static_ideal_generator_159 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 159, permutation);  generators.emplace_front(static_ideal_generator_159); \
	case 158:  static poly static_ideal_generator_158 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 158, permutation);  generators.emplace_front(static_ideal_generator_158); \
	case 157:  static poly static_ideal_generator_157 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 157, permutation);  generators.emplace_front(static_ideal_generator_157); \
	case 156:  static poly static_ideal_generator_156 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 156, permutation);  generators.emplace_front(static_ideal_generator_156); \
	case 155:  static poly static_ideal_generator_155 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 155, permutation);  generators.emplace_front(static_ideal_generator_155); \
	case 154:  static poly static_ideal_generator_154 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 154, permutation);  generators.emplace_front(static_ideal_generator_154); \
	case 153:  static poly static_ideal_generator_153 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 153, permutation);  generators.emplace_front(static_ideal_generator_153); \
	case 152:  static poly static_ideal_generator_152 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 152, permutation);  generators.emplace_front(static_ideal_generator_152); \
	case 151:  static poly static_ideal_generator_151 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 151, permutation);  generators.emplace_front(static_ideal_generator_151); \
	case 150:  static poly static_ideal_generator_150 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 150, permutation);  generators.emplace_front(static_ideal_generator_150); \
	case 149:  static poly static_ideal_generator_149 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 149, permutation);  generators.emplace_front(static_ideal_generator_149); \
	case 148:  static poly static_ideal_generator_148 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 148, permutation);  generators.emplace_front(static_ideal_generator_148); \
	case 147:  static poly static_ideal_generator_147 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 147, permutation);  generators.emplace_front(static_ideal_generator_147); \
	case 146:  static poly static_ideal_generator_146 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 146, permutation);  generators.emplace_front(static_ideal_generator_146); \
	case 145:  static poly static_ideal_generator_145 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 145, permutation);  generators.emplace_front(static_ideal_generator_145); \
	case 144:  static poly static_ideal_generator_144 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 144, permutation);  generators.emplace_front(static_ideal_generator_144); \
	case 143:  static poly static_ideal_generator_143 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 143, permutation);  generators.emplace_front(static_ideal_generator_143); \
	case 142:  static poly static_ideal_generator_142 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 142, permutation);  generators.emplace_front(static_ideal_generator_142); \
	case 141:  static poly static_ideal_generator_141 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 141, permutation);  generators.emplace_front(static_ideal_generator_141); \
	case 140:  static poly static_ideal_generator_140 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 140, permutation);  generators.emplace_front(static_ideal_generator_140); \
	case 139:  static poly static_ideal_generator_139 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 139, permutation);  generators.emplace_front(static_ideal_generator_139); \
	case 138:  static poly static_ideal_generator_138 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 138, permutation);  generators.emplace_front(static_ideal_generator_138); \
	case 137:  static poly static_ideal_generator_137 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 137, permutation);  generators.emplace_front(static_ideal_generator_137); \
	case 136:  static poly static_ideal_generator_136 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 136, permutation);  generators.emplace_front(static_ideal_generator_136); \
	case 135:  static poly static_ideal_generator_135 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 135, permutation);  generators.emplace_front(static_ideal_generator_135); \
	case 134:  static poly static_ideal_generator_134 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 134, permutation);  generators.emplace_front(static_ideal_generator_134); \
	case 133:  static poly static_ideal_generator_133 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 133, permutation);  generators.emplace_front(static_ideal_generator_133); \
	case 132:  static poly static_ideal_generator_132 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 132, permutation);  generators.emplace_front(static_ideal_generator_132); \
	case 131:  static poly static_ideal_generator_131 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 131, permutation);  generators.emplace_front(static_ideal_generator_131); \
	case 130:  static poly static_ideal_generator_130 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 130, permutation);  generators.emplace_front(static_ideal_generator_130); \
	case 129:  static poly static_ideal_generator_129 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 129, permutation);  generators.emplace_front(static_ideal_generator_129); \
	case 128:  static poly static_ideal_generator_128 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 128, permutation);  generators.emplace_front(static_ideal_generator_128); \
	case 127:  static poly static_ideal_generator_127 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 127, permutation);  generators.emplace_front(static_ideal_generator_127); \
	case 126:  static poly static_ideal_generator_126 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 126, permutation);  generators.emplace_front(static_ideal_generator_126); \
	case 125:  static poly static_ideal_generator_125 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 125, permutation);  generators.emplace_front(static_ideal_generator_125); \
	case 124:  static poly static_ideal_generator_124 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 124, permutation);  generators.emplace_front(static_ideal_generator_124); \
	case 123:  static poly static_ideal_generator_123 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 123, permutation);  generators.emplace_front(static_ideal_generator_123); \
	case 122:  static poly static_ideal_generator_122 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 122, permutation);  generators.emplace_front(static_ideal_generator_122); \
	case 121:  static poly static_ideal_generator_121 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 121, permutation);  generators.emplace_front(static_ideal_generator_121); \
	case 120:  static poly static_ideal_generator_120 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 120, permutation);  generators.emplace_front(static_ideal_generator_120); \
	case 119:  static poly static_ideal_generator_119 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 119, permutation);  generators.emplace_front(static_ideal_generator_119); \
	case 118:  static poly static_ideal_generator_118 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 118, permutation);  generators.emplace_front(static_ideal_generator_118); \
	case 117:  static poly static_ideal_generator_117 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 117, permutation);  generators.emplace_front(static_ideal_generator_117); \
	case 116:  static poly static_ideal_generator_116 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 116, permutation);  generators.emplace_front(static_ideal_generator_116); \
	case 115:  static poly static_ideal_generator_115 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 115, permutation);  generators.emplace_front(static_ideal_generator_115); \
	case 114:  static poly static_ideal_generator_114 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 114, permutation);  generators.emplace_front(static_ideal_generator_114); \
	case 113:  static poly static_ideal_generator_113 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 113, permutation);  generators.emplace_front(static_ideal_generator_113); \
	case 112:  static poly static_ideal_generator_112 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 112, permutation);  generators.emplace_front(static_ideal_generator_112); \
	case 111:  static poly static_ideal_generator_111 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 111, permutation);  generators.emplace_front(static_ideal_generator_111); \
	case 110:  static poly static_ideal_generator_110 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 110, permutation);  generators.emplace_front(static_ideal_generator_110); \
	case 109:  static poly static_ideal_generator_109 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 109, permutation);  generators.emplace_front(static_ideal_generator_109); \
	case 108:  static poly static_ideal_generator_108 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 108, permutation);  generators.emplace_front(static_ideal_generator_108); \
	case 107:  static poly static_ideal_generator_107 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 107, permutation);  generators.emplace_front(static_ideal_generator_107); \
	case 106:  static poly static_ideal_generator_106 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 106, permutation);  generators.emplace_front(static_ideal_generator_106); \
	case 105:  static poly static_ideal_generator_105 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 105, permutation);  generators.emplace_front(static_ideal_generator_105); \
	case 104:  static poly static_ideal_generator_104 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 104, permutation);  generators.emplace_front(static_ideal_generator_104); \
	case 103:  static poly static_ideal_generator_103 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 103, permutation);  generators.emplace_front(static_ideal_generator_103); \
	case 102:  static poly static_ideal_generator_102 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 102, permutation);  generators.emplace_front(static_ideal_generator_102); \
	case 101:  static poly static_ideal_generator_101 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 101, permutation);  generators.emplace_front(static_ideal_generator_101); \
	case 100:  static poly static_ideal_generator_100 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 100, permutation);  generators.emplace_front(static_ideal_generator_100); \
	case 99:  static poly static_ideal_generator_99 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 99, permutation);  generators.emplace_front(static_ideal_generator_99); \
	case 98:  static poly static_ideal_generator_98 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 98, permutation);  generators.emplace_front(static_ideal_generator_98); \
	case 97:  static poly static_ideal_generator_97 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 97, permutation);  generators.emplace_front(static_ideal_generator_97); \
	case 96:  static poly static_ideal_generator_96 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 96, permutation);  generators.emplace_front(static_ideal_generator_96); \
	case 95:  static poly static_ideal_generator_95 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 95, permutation);  generators.emplace_front(static_ideal_generator_95); \
	case 94:  static poly static_ideal_generator_94 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 94, permutation);  generators.emplace_front(static_ideal_generator_94); \
	case 93:  static poly static_ideal_generator_93 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 93, permutation);  generators.emplace_front(static_ideal_generator_93); \
	case 92:  static poly static_ideal_generator_92 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 92, permutation);  generators.emplace_front(static_ideal_generator_92); \
	case 91:  static poly static_ideal_generator_91 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 91, permutation);  generators.emplace_front(static_ideal_generator_91); \
	case 90:  static poly static_ideal_generator_90 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 90, permutation);  generators.emplace_front(static_ideal_generator_90); \
	case 89:  static poly static_ideal_generator_89 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 89, permutation);  generators.emplace_front(static_ideal_generator_89); \
	case 88:  static poly static_ideal_generator_88 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 88, permutation);  generators.emplace_front(static_ideal_generator_88); \
	case 87:  static poly static_ideal_generator_87 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 87, permutation);  generators.emplace_front(static_ideal_generator_87); \
	case 86:  static poly static_ideal_generator_86 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 86, permutation);  generators.emplace_front(static_ideal_generator_86); \
	case 85:  static poly static_ideal_generator_85 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 85, permutation);  generators.emplace_front(static_ideal_generator_85); \
	case 84:  static poly static_ideal_generator_84 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 84, permutation);  generators.emplace_front(static_ideal_generator_84); \
	case 83:  static poly static_ideal_generator_83 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 83, permutation);  generators.emplace_front(static_ideal_generator_83); \
	case 82:  static poly static_ideal_generator_82 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 82, permutation);  generators.emplace_front(static_ideal_generator_82); \
	case 81:  static poly static_ideal_generator_81 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 81, permutation);  generators.emplace_front(static_ideal_generator_81); \
	case 80:  static poly static_ideal_generator_80 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 80, permutation);  generators.emplace_front(static_ideal_generator_80); \
	case 79:  static poly static_ideal_generator_79 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 79, permutation);  generators.emplace_front(static_ideal_generator_79); \
	case 78:  static poly static_ideal_generator_78 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 78, permutation);  generators.emplace_front(static_ideal_generator_78); \
	case 77:  static poly static_ideal_generator_77 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 77, permutation);  generators.emplace_front(static_ideal_generator_77); \
	case 76:  static poly static_ideal_generator_76 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 76, permutation);  generators.emplace_front(static_ideal_generator_76); \
	case 75:  static poly static_ideal_generator_75 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 75, permutation);  generators.emplace_front(static_ideal_generator_75); \
	case 74:  static poly static_ideal_generator_74 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 74, permutation);  generators.emplace_front(static_ideal_generator_74); \
	case 73:  static poly static_ideal_generator_73 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 73, permutation);  generators.emplace_front(static_ideal_generator_73); \
	case 72:  static poly static_ideal_generator_72 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 72, permutation);  generators.emplace_front(static_ideal_generator_72); \
	case 71:  static poly static_ideal_generator_71 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 71, permutation);  generators.emplace_front(static_ideal_generator_71); \
	case 70:  static poly static_ideal_generator_70 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 70, permutation);  generators.emplace_front(static_ideal_generator_70); \
	case 69:  static poly static_ideal_generator_69 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 69, permutation);  generators.emplace_front(static_ideal_generator_69); \
	case 68:  static poly static_ideal_generator_68 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 68, permutation);  generators.emplace_front(static_ideal_generator_68); \
	case 67:  static poly static_ideal_generator_67 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 67, permutation);  generators.emplace_front(static_ideal_generator_67); \
	case 66:  static poly static_ideal_generator_66 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 66, permutation);  generators.emplace_front(static_ideal_generator_66); \
	case 65:  static poly static_ideal_generator_65 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 65, permutation);  generators.emplace_front(static_ideal_generator_65); \
	case 64:  static poly static_ideal_generator_64 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 64, permutation);  generators.emplace_front(static_ideal_generator_64); \
	case 63:  static poly static_ideal_generator_63 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 63, permutation);  generators.emplace_front(static_ideal_generator_63); \
	case 62:  static poly static_ideal_generator_62 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 62, permutation);  generators.emplace_front(static_ideal_generator_62); \
	case 61:  static poly static_ideal_generator_61 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 61, permutation);  generators.emplace_front(static_ideal_generator_61); \
	case 60:  static poly static_ideal_generator_60 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 60, permutation);  generators.emplace_front(static_ideal_generator_60); \
	case 59:  static poly static_ideal_generator_59 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 59, permutation);  generators.emplace_front(static_ideal_generator_59); \
	case 58:  static poly static_ideal_generator_58 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 58, permutation);  generators.emplace_front(static_ideal_generator_58); \
	case 57:  static poly static_ideal_generator_57 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 57, permutation);  generators.emplace_front(static_ideal_generator_57); \
	case 56:  static poly static_ideal_generator_56 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 56, permutation);  generators.emplace_front(static_ideal_generator_56); \
	case 55:  static poly static_ideal_generator_55 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 55, permutation);  generators.emplace_front(static_ideal_generator_55); \
	case 54:  static poly static_ideal_generator_54 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 54, permutation);  generators.emplace_front(static_ideal_generator_54); \
	case 53:  static poly static_ideal_generator_53 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 53, permutation);  generators.emplace_front(static_ideal_generator_53); \
	case 52:  static poly static_ideal_generator_52 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 52, permutation);  generators.emplace_front(static_ideal_generator_52); \
	case 51:  static poly static_ideal_generator_51 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 51, permutation);  generators.emplace_front(static_ideal_generator_51); \
	case 50:  static poly static_ideal_generator_50 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 50, permutation);  generators.emplace_front(static_ideal_generator_50); \
	case 49:  static poly static_ideal_generator_49 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 49, permutation);  generators.emplace_front(static_ideal_generator_49); \
	case 48:  static poly static_ideal_generator_48 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 48, permutation);  generators.emplace_front(static_ideal_generator_48); \
	case 47:  static poly static_ideal_generator_47 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 47, permutation);  generators.emplace_front(static_ideal_generator_47); \
	case 46:  static poly static_ideal_generator_46 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 46, permutation);  generators.emplace_front(static_ideal_generator_46); \
	case 45:  static poly static_ideal_generator_45 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 45, permutation);  generators.emplace_front(static_ideal_generator_45); \
	case 44:  static poly static_ideal_generator_44 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 44, permutation);  generators.emplace_front(static_ideal_generator_44); \
	case 43:  static poly static_ideal_generator_43 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 43, permutation);  generators.emplace_front(static_ideal_generator_43); \
	case 42:  static poly static_ideal_generator_42 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 42, permutation);  generators.emplace_front(static_ideal_generator_42); \
	case 41:  static poly static_ideal_generator_41 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 41, permutation);  generators.emplace_front(static_ideal_generator_41); \
	case 40:  static poly static_ideal_generator_40 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 40, permutation);  generators.emplace_front(static_ideal_generator_40); \
	case 39:  static poly static_ideal_generator_39 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 39, permutation);  generators.emplace_front(static_ideal_generator_39); \
	case 38:  static poly static_ideal_generator_38 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 38, permutation);  generators.emplace_front(static_ideal_generator_38); \
	case 37:  static poly static_ideal_generator_37 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 37, permutation);  generators.emplace_front(static_ideal_generator_37); \
	case 36:  static poly static_ideal_generator_36 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 36, permutation);  generators.emplace_front(static_ideal_generator_36); \
	case 35:  static poly static_ideal_generator_35 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 35, permutation);  generators.emplace_front(static_ideal_generator_35); \
	case 34:  static poly static_ideal_generator_34 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 34, permutation);  generators.emplace_front(static_ideal_generator_34); \
	case 33:  static poly static_ideal_generator_33 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 33, permutation);  generators.emplace_front(static_ideal_generator_33); \
	case 32:  static poly static_ideal_generator_32 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 32, permutation);  generators.emplace_front(static_ideal_generator_32); \
	case 31:  static poly static_ideal_generator_31 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 31, permutation);  generators.emplace_front(static_ideal_generator_31); \
	case 30:  static poly static_ideal_generator_30 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 30, permutation);  generators.emplace_front(static_ideal_generator_30); \
	case 29:  static poly static_ideal_generator_29 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 29, permutation);  generators.emplace_front(static_ideal_generator_29); \
	case 28:  static poly static_ideal_generator_28 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 28, permutation);  generators.emplace_front(static_ideal_generator_28); \
	case 27:  static poly static_ideal_generator_27 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 27, permutation);  generators.emplace_front(static_ideal_generator_27); \
	case 26:  static poly static_ideal_generator_26 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 26, permutation);  generators.emplace_front(static_ideal_generator_26); \
	case 25:  static poly static_ideal_generator_25 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 25, permutation);  generators.emplace_front(static_ideal_generator_25); \
	case 24:  static poly static_ideal_generator_24 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 24, permutation);  generators.emplace_front(static_ideal_generator_24); \
	case 23:  static poly static_ideal_generator_23 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 23, permutation);  generators.emplace_front(static_ideal_generator_23); \
	case 22:  static poly static_ideal_generator_22 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 22, permutation);  generators.emplace_front(static_ideal_generator_22); \
	case 21:  static poly static_ideal_generator_21 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 21, permutation);  generators.emplace_front(static_ideal_generator_21); \
	case 20:  static poly static_ideal_generator_20 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 20, permutation);  generators.emplace_front(static_ideal_generator_20); \
	case 19:  static poly static_ideal_generator_19 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 19, permutation);  generators.emplace_front(static_ideal_generator_19); \
	case 18:  static poly static_ideal_generator_18 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 18, permutation);  generators.emplace_front(static_ideal_generator_18); \
	case 17:  static poly static_ideal_generator_17 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 17, permutation);  generators.emplace_front(static_ideal_generator_17); \
	case 16:  static poly static_ideal_generator_16 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 16, permutation);  generators.emplace_front(static_ideal_generator_16); \
	case 15:  static poly static_ideal_generator_15 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 15, permutation);  generators.emplace_front(static_ideal_generator_15); \
	case 14:  static poly static_ideal_generator_14 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 14, permutation);  generators.emplace_front(static_ideal_generator_14); \
	case 13:  static poly static_ideal_generator_13 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 13, permutation);  generators.emplace_front(static_ideal_generator_13); \
	case 12:  static poly static_ideal_generator_12 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 12, permutation);  generators.emplace_front(static_ideal_generator_12); \
	case 11:  static poly static_ideal_generator_11 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 11, permutation);  generators.emplace_front(static_ideal_generator_11); \
	case 10:  static poly static_ideal_generator_10 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 10, permutation);  generators.emplace_front(static_ideal_generator_10); \
	case 9:  static poly static_ideal_generator_9 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 9, permutation);  generators.emplace_front(static_ideal_generator_9); \
	case 8:  static poly static_ideal_generator_8 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 8, permutation);  generators.emplace_front(static_ideal_generator_8); \
	case 7:  static poly static_ideal_generator_7 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 7, permutation);  generators.emplace_front(static_ideal_generator_7); \
	case 6:  static poly static_ideal_generator_6 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 6, permutation);  generators.emplace_front(static_ideal_generator_6); \
	case 5:  static poly static_ideal_generator_5 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 5, permutation);  generators.emplace_front(static_ideal_generator_5); \
	case 4:  static poly static_ideal_generator_4 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 4, permutation);  generators.emplace_front(static_ideal_generator_4); \
	case 3:  static poly static_ideal_generator_3 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 3, permutation);  generators.emplace_front(static_ideal_generator_3); \
	case 2:  static poly static_ideal_generator_2 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 2, permutation);  generators.emplace_front(static_ideal_generator_2); \
	case 1:  static poly static_ideal_generator_1 = RESOLVE_INTERFACE_FUNCTION(read_generator) (base_filename, 1, permutation);  generators.emplace_front(static_ideal_generator_1); \
}

#define READ_GENERATORS_STATICALLY(generators, r) \
  std::vector<int> permutation; \
  for (int k=1; k<=r; k++) {permutation.emplace_back(k);} \
  READ_GENERATORS_STATICALLY_WITH_PERMUTATION(generators, r, permutation);


#define RESOLVE_INTERFACE_FUNCTION(function) \
      (fhg::util::scoped_dlhandle \
      (config::implementation(), \
      RTLD_GLOBAL | RTLD_NOW | RTLD_DEEPBIND) \
      .sym<decltype(function)> \
      (BOOST_PP_STRINGIZE(function)))



typedef struct spolyrec *    poly;


// types used by GPI-Space if you set the type of a place (or an "out-many" port) to "list", "set" or "map":
using GpiVariant = pnet::type::value::value_type;
using GpiStruct  = pnet::type::value::structured_type;
using GpiList    = std::list<GpiVariant>;
using GpiSet     = std::set<GpiVariant>;
using GpiMap     = std::map<GpiVariant,GpiVariant>;

// visitor functions to convert to proper lists, sets and maps:
template <typename T>
class variant_visitor : public boost::static_visitor<T&>
{
public:
  T& operator() (T& data) const
  {
    return data;
  }

  template <typename U>
  T& operator() (U&) const
  {
   static T instance{};
   return instance;
  }
};

inline GpiList& get_list(GpiVariant& v) {return boost::apply_visitor(variant_visitor<GpiList>(), v);}
inline GpiSet&  get_set (GpiVariant& v) {return boost::apply_visitor(variant_visitor<GpiSet >(), v);}
inline GpiMap&  get_map (GpiVariant& v) {return boost::apply_visitor(variant_visitor<GpiMap >(), v);}

inline GpiList const& get_list(GpiVariant  const& v) {return boost::apply_visitor(variant_visitor<GpiList const>(), v);}
inline GpiSet const&  get_set (GpiVariant  const& v) {return boost::apply_visitor(variant_visitor<GpiSet  const>(), v);}
inline GpiMap const&  get_map (GpiVariant  const& v) {return boost::apply_visitor(variant_visitor<GpiMap  const>(), v);}

class print_variant_visitor : public boost::static_visitor<void>
{
private:
  int depth;
public:
  print_variant_visitor(int const& depth=0)
  {
    this->depth = depth;
  }
  void operator() (const we::type::literal::control&) const {
    std::string whitespace(this->depth * 4, ' ');
    std::cout << whitespace << "CONTROL_TOKEN";
  }
  void operator() (const GpiList& data) const {
    std::string whitespace(this->depth * 4, ' ');
    std::cout << whitespace << "List of " << data.size() << " elements:" << std::endl;
    for(GpiList::const_iterator it=data.begin(); it!=data.end(); ++it)
      {boost::apply_visitor(print_variant_visitor(this->depth + 1), *it);}
  }
  void operator() (const GpiSet& data) const {
    std::string whitespace(this->depth * 4, ' ');
    std::cout << whitespace << "Set of " << data.size() << " elements:" << std::endl;
    for(GpiSet::const_iterator it=data.begin(); it!=data.end(); ++it)
      {boost::apply_visitor(print_variant_visitor(this->depth + 1), *it);}
  }
  void operator() (const GpiMap& data) const {
    std::string whitespace(this->depth * 4, ' ');
    std::cout << whitespace << "Map of " << data.size() << " elements:" << std::endl;
    for(GpiMap::const_iterator it=data.begin(); it!=data.end(); ++it) {
      boost::apply_visitor(print_variant_visitor(this->depth + 1), it->first);
      boost::apply_visitor(print_variant_visitor(this->depth + 1), it->second);
    }
  }
  void operator() (const bool& data) const {
    std::string whitespace(this->depth * 4, ' ');
    std::cout << whitespace << data << std::endl;
  }
  void operator() (const int& data) const {
    std::string whitespace(this->depth * 4, ' ');
    std::cout << whitespace << data << "   (int)" << std::endl;
  }
  void operator() (const long& data) const {
    std::string whitespace(this->depth * 4, ' ');
    std::cout << whitespace << data << "   (long)" << std::endl;
  }
  void operator() (const std::string& data) const {
    std::string whitespace(this->depth * 4, ' ');
    std::cout << whitespace << data << "   (string)" << std::endl;
  }

  template <typename U>
  void operator() (const U&) const {
    std::string whitespace(this->depth * 4, ' ');
    std::cout << whitespace << "type not implemented" << std::endl;
  }
};

inline void print_variant(GpiVariant const& v, int depth=0) {boost::apply_visitor(print_variant_visitor(depth), v);}



// helper functions:

inline GpiList lcm_monom(GpiVariant const& m1, GpiVariant const& m2)
{
  GpiList M1 = get_list(m1);
  GpiList M2 = get_list(m2);
  GpiList res;
  if(M1.size()!=M2.size()) {throw std::runtime_error ("exponent vectors have different lengths in lcm_monom");}
  GpiList::iterator it1 = M1.begin();
  GpiList::iterator it2 = M2.begin();
  for(; it1 != M1.end(); ++it1, ++it2)
  {
    res.emplace_back(std::max(boost::get<long>(*it1), boost::get<long>(*it2)));
  }
  return res;
}
inline GpiList lcm(GpiVariant const& t1, GpiVariant const& t2)
{
  GpiList T1 = get_list(t1);
  GpiList T2 = get_list(t2);
  GpiVariant m1 = T1.front();
  GpiVariant m2 = T2.front();
  long T1_comp = boost::get<long>(*std::next(T1.begin()));
  long T2_comp = boost::get<long>(*std::next(T2.begin()));
  GpiList res;
  if(T1_comp==T2_comp)
  {
    res.emplace_back(lcm_monom(m1,m2));
    res.emplace_back(T1_comp);
  }
  return res; // will be empty if T1,T2 are in different components!
}
/*
inline long deg(GpiVariant const& m)
{
  GpiList M = get_list(get_list(m).front());
  if(M.size()==0) {return(-1);}
  long d=0;
  for(GpiList::iterator it = M.begin(); it != M.end(); ++it)
  {
    d += boost::get<long>(*it);
  }
  return d;
}
*/
inline bool coprime_monom(GpiVariant const& m1, GpiVariant const& m2)
{
  GpiList M1 = get_list(m1);
  GpiList M2 = get_list(m2);
  if(M1.size()!=M2.size()) {throw std::runtime_error ("exponent vectors have different lengths in coprime_monom");}
  GpiList::iterator it1 = M1.begin();
  GpiList::iterator it2 = M2.begin();
  for(; it1 != M1.end(); ++it1, ++it2)
  {
    if(boost::get<long>(*it1) && boost::get<long>(*it2)) {return false;}
  }
  return true;
}
inline bool divides_monom(GpiVariant const& m1, GpiVariant const& m2)
{
  GpiList M1 = get_list(m1);
  GpiList M2 = get_list(m2);
  if(M1.size()!=M2.size()) {throw std::runtime_error ("exponent vectors have different lengths in divides_monom");}
  GpiList::iterator it1 = M1.begin();
  GpiList::iterator it2 = M2.begin();
  for(; it1 != M1.end(); ++it1, ++it2)
  {
    if(boost::get<long>(*it1) > boost::get<long>(*it2)) {return false;}
  }
  return true;
}
inline bool dp_larger_equal(GpiList const& T1, GpiList const& T2, int d1, int d2)
{
  if(T1.size()!=T2.size()) {throw std::runtime_error ("exponent vectors have different lengths in dp_larger_equal");}
  if(d1>d2) {return true;}
  if(d1<d2) {return false;}

  GpiList::const_reverse_iterator it1 = T1.rbegin();
  GpiList::const_reverse_iterator it2 = T2.rbegin();
  int T1_comp = boost::get<int>(*it1); ++it1;
  int T2_comp = boost::get<int>(*it2); ++it2;
  for(; it1 != T1.rend(); ++it1, ++it2)
  {
    int exp1 = boost::get<int>(*it1);
    int exp2 = boost::get<int>(*it2);
    //std::cout << "       " << exp1 << "  " << exp2 << std::endl;
    if(exp1<exp2) {return true;}
    if(exp1>exp2) {return false;}
  }
  if(T1_comp>T2_comp) {return true;}
  if(T1_comp<T2_comp) {return false;}

  return true;
}
/*
inline bool dp_larger_equal(GpiVariant const& t1, GpiVariant const& t2)
{
  GpiList T1 = get_list(t1);
  GpiList T2 = get_list(t2);

  GpiList M1 = get_list(T1.front());
  GpiList M2 = get_list(T2.front());
  if(M1.size()!=M2.size()) {throw std::runtime_error ("exponent vectors have different lengths in equal");}
  long d1 = deg(t1);
  long d2 = deg(t2);
  if(d1>d2) {return true;}
  if(d1<d2) {return false;}

  GpiList::reverse_iterator it1 = M1.rbegin();
  GpiList::reverse_iterator it2 = M2.rbegin();
  for(; it1 != M1.rend(); ++it1, ++it2)
  {
    int exp1 = boost::get<long>(*it1);
    int exp2 = boost::get<long>(*it2);
    //std::cout << "       " << exp1 << "  " << exp2 << std::endl;
    if(exp1<exp2) {return true;}
    if(exp1>exp2) {return false;}
  }

  long T1_comp = boost::get<long>(*std::next(T1.begin()));
  long T2_comp = boost::get<long>(*std::next(T2.begin()));
  if(T1_comp>T2_comp) {return true;}
  if(T1_comp<T2_comp) {return false;}

  return true;
}
*/
inline bool equal(GpiVariant const& t1, GpiVariant const& t2)
{
  GpiList T1 = get_list(t1);
  GpiList T2 = get_list(t2);

  GpiList M1 = get_list(T1.front());
  GpiList M2 = get_list(T2.front());
  if(M1.size()!=M2.size()) {throw std::runtime_error ("exponent vectors have different lengths in equal");}

  GpiList::reverse_iterator it1 = M1.rbegin();
  GpiList::reverse_iterator it2 = M2.rbegin();
  for(; it1 != M1.rend(); ++it1, ++it2)
  {
    int exp1 = boost::get<long>(*it1);
    int exp2 = boost::get<long>(*it2);
    if(exp1!=exp2) {return false;}
  }

  long T1_comp = boost::get<long>(*std::next(T1.begin()));
  long T2_comp = boost::get<long>(*std::next(T2.begin()));
  if(T1_comp!=T2_comp) {return false;}

  return true;
}

inline GpiList vec2list(std::vector<int> vec)
{
  GpiList L;
  for (std::vector<int>::iterator it=vec.begin(); it!=vec.end(); ++it)
  {
    L.emplace_back(*it);
  }
  return L;
}

inline std::vector<int> list2vec(GpiList L)
{
  std::vector<int> vec;
  for (GpiList::iterator it=L.begin(); it!=L.end(); ++it)
  {
    vec.emplace_back(boost::get<int>(*it));
  }
  return vec;
}

inline std::vector<int> list2vec_long(GpiList L)
{
  std::vector<int> vec;
  for (GpiList::iterator it=L.begin(); it!=L.end(); ++it)
  {
    vec.emplace_back((int) boost::get<long>(*it));
  }
  return vec;
}

inline std::vector<int> list2vec(GpiVariant L)
{
  return list2vec(get_list(L));
}


// call like: chain_criterion(i, j, A, kill_indices, M, get_list(*std::next(M.begin(),i)), get_list(*std::next(M.begin(),j)));

/*
inline void chain_criterion(int const& i, int const& j, GpiMap& A, GpiList& kill_indices, GpiList const& M, GpiList const& Mi, GpiList const& Mj)
{
  long Mi_comp = boost::get<long>(*std::next(Mi.begin()));
  long Mj_comp = boost::get<long>(*std::next(Mj.begin()));
  if(Mi_comp==Mj_comp)
  {
    int r = M.size();
    int entry;
    GpiList::const_iterator it = M.begin();
    for(int k=1; k<=r; k++, ++it)
    {
      GpiList Mk = get_list(*it);
      long Mk_comp = boost::get<long>(*std::next(Mk.begin()));
      if(k!=i && k!=j && Mi_comp==Mk_comp)
      {
        //row
        entry = boost::get<int>(get_list(A[GpiSet({i,k})]).front());  // A[i,k]
        if(EARLY_CHAIN_CRIT || entry>=2 || entry==NF_IS_ZERO || entry==PRODUCT_CRITERION || entry==CHAIN_CRITERION || entry==DIFFERENT_COMPONENTS)
        {
          if(divides_monom(Mi.front(), get_list(*std::next(get_list(A[GpiSet({k,j})]).begin())).front()))  // chain-criterion: if (i,k) finished as well and mi divides lcm(mj,mk) then (k,j) can be discarded
          {
            entry = boost::get<int>(get_list(A[GpiSet({k,j})]).front()); // A[k,j]
            if(entry == STARTED)
            {
              if(k>j) {kill_indices.emplace_back(std::to_string(k)+","+std::to_string(j));}
              else    {kill_indices.emplace_back(std::to_string(j)+","+std::to_string(k));}
            }
            if(entry == STARTED || entry == 0) // DEBUG!
            {
              get_list(A[GpiSet({k,j})]).front() = CHAIN_CRITERION;
              chain_criterion(k, j, A, kill_indices, M, Mk, Mj);
            }
          }
        }
        //column
        entry = boost::get<int>(get_list(A[GpiSet({k,j})]).front()); // A[k,j]
        if(EARLY_CHAIN_CRIT || entry>=2 || entry==NF_IS_ZERO || entry==PRODUCT_CRITERION || entry==CHAIN_CRITERION || entry==DIFFERENT_COMPONENTS)
        {
          if(divides_monom(Mj.front(), get_list(*std::next(get_list(A[GpiSet({i,k})]).begin())).front()))  // chain-criterion: if (k,j) finished as well and mi divides lcm(mj,mk) then (i,k) can be discarded
          {
            entry = boost::get<int>(get_list(A[GpiSet({i,k})]).front()); // A[i,k]
            if(entry == STARTED)
            {
              if(i>k) {kill_indices.emplace_back(std::to_string(i)+","+std::to_string(k));}
              else    {kill_indices.emplace_back(std::to_string(k)+","+std::to_string(i));}
            }
            if(entry == STARTED || entry == 0) // DEBUG!
            {
              get_list(A[GpiSet({i,k})]).front() = CHAIN_CRITERION;
              chain_criterion(i, k, A, kill_indices, M, Mi, Mk);
            }
          }
        }
      }
    }
  }
}
*/

// call like: chain_criterion(i, j, A, M, get_list(*std::next(M.begin(),i)), get_list(*std::next(M.begin(),j)));
// with i<j, will test if s-pair (i,j) can be skipped due to a third index k<max(i,j), k!=i, k!=j;
/*
inline int chain_criterion(int const& i, int const& j, GpiVariant const& lcm_i_j, GpiList const& M, GpiList const& Mi, GpiList const& Mj, long& chaincrit_counter)
{
  long Mi_comp = boost::get<long>(*std::next(Mi.begin()));
  long Mj_comp = boost::get<long>(*std::next(Mj.begin()));
  //std::cout << "lcm_i_j: " << std::endl; print_variant(lcm_i_j);

  if(Mi_comp==Mj_comp)
  {
    GpiList::const_iterator it = M.begin();
    for(int k=1; k<std::min(i,j); k++, ++it)
    {
      GpiList Mk = get_list(*it);
      long Mk_comp = boost::get<long>(*std::next(Mk.begin()));
      if(k!=i && k!=j && Mi_comp==Mk_comp)
      {
        if(divides_monom(Mk.front(), lcm_i_j))  // chain-criterion: if mk divides lcm(mi,mj) then (i,j) can be discarded
        {
          //runtime[(std::string) "CHAIN CRITERION"] = GpiList({0L, 0L, 1L, 1L});
          chaincrit_counter++;
          return CHAIN_CRITERION;
        }
      }
    }
  }
  return 0;
}
*/
/*
inline int product_criterion(GpiList const& Mi, GpiList const& Mj, long& prodcrit_counter)
{
  long Mi_comp = boost::get<long>(*std::next(Mi.begin()));
  long Mj_comp = boost::get<long>(*std::next(Mj.begin()));
  if(Mi_comp==0) // ideals & polynomials
  {
    if(coprime_monom(Mi.front(), Mj.front()))  // product-criterion: if mi and mj are coprime then (i,j) can be discarded
    {
      //runtime[(std::string) "PRODUCT CRITERION"] = GpiList({0L, 0L, 1L, 1L});
      prodcrit_counter++;
      return PRODUCT_CRITERION;
    }
  }
  else // modules & vectors
  {
    if(Mi_comp!=Mj_comp) // for modules: test instead if the leads lie in different components (thus spoly=0 trivially)
    {
      //runtime[(std::string) "LEADS IN DIFFERENT COMPONENTS"] = GpiList({0L, 0L, 1L, 1L});
      prodcrit_counter++;
      return DIFFERENT_COMPONENTS;
    }
  }
  return 0;
}

inline int apply_criteria(int const& i, int const& j, GpiVariant const& lcm_i_j, GpiList const& M, GpiList const& Mi, GpiList const& Mj, long& prodcrit_counter, long& chaincrit_counter)
{
  int state = product_criterion(Mi, Mj, prodcrit_counter);
  if(state==0) {state = chain_criterion(i, j, lcm_i_j, M, Mi, Mj, chaincrit_counter);}
  return state;
}

*/


// STD::VECTOR based functions //
inline std::vector<int> lcm(std::vector<int> const& m1, std::vector<int> const& m2)
{
  std::vector<int>::const_iterator it1 = m1.begin();
  std::vector<int>::const_iterator it2 = m2.begin();
  std::vector<int> res;
  for(; it1 != m1.end(); ++it1, ++it2)
  {
    res.emplace_back(std::max(*it1, *it2));
  }
  return res;
}
inline bool equal(std::vector<int> const& m1, std::vector<int> const& m2)
{
  std::vector<int>::const_iterator it1 = m1.begin();
  std::vector<int>::const_iterator it2 = m2.begin();
  std::vector<int> res;
  for(; it1 != m1.end(); ++it1, ++it2)
  {
    if(*it1!=*it2) {return false;}
  }
  return true;
}
inline bool divides_monom(std::vector<int> const& m1, std::vector<int> const& m2)
{
  if (m1.back()!=m2.back()) {return false;} // different components
  std::vector<int>::const_iterator it1 = m1.begin();
  std::vector<int>::const_iterator it2 = m2.begin();
  std::vector<int> res;
  for(; it1 != m1.end(); ++it1, ++it2)
  {
    if(*it1>*it2) {return false;}
  }
  return true;
}
inline bool coprime_monom(std::vector<int> const& m1, std::vector<int> const& m2)
{
  if (m1.back()!=m2.back()) {return false;} // different components
  std::vector<int>::const_iterator it1 = m1.begin();
  std::vector<int>::const_iterator it2 = m2.begin();
  std::vector<int> res;
  for(; it1 != m1.end(); ++it1, ++it2)
  {
    if(*it1!=0 && *it2!=0) {return false;}
  }
  return true;
}
inline int deg(std::vector<int> const& monom)
{
  int d = 0;
  int n = monom.size()-1;
  for(std::vector<int>::const_iterator it = monom.begin(); n>0; ++it)
  {
    d += (*it);
    n--;
  }
  return d;
}
inline bool test_PC(std::vector<int> const& Mi, std::vector<int> const& Mj)
{
  int Mi_comp = Mi.back();
  int Mj_comp = Mj.back();
  if(Mi_comp==0) // ideals & polynomials
  {
    return coprime_monom(Mi, Mj); // product-criterion: if mi and mj are coprime then (i,j) can be discarded
  }
  else // modules & vectors
  {
    return (Mi_comp!=Mj_comp); // for modules: test instead if the leads lie in different components (thus spoly=0 trivially)
  }
  return false;
}
inline bool test_CC(std::vector<int> const& lcm_i_j, std::vector<int> const& Mi, std::vector<int> const& Mj, std::vector<int> const& Mk)
{
  int Mi_comp = Mi.back();
  int Mj_comp = Mj.back();
  int Mk_comp = Mk.back();

  if(Mi_comp==Mj_comp && Mi_comp==Mk_comp)
  {
    return (divides_monom(Mk, lcm_i_j));  // chain-criterion: if mk divides lcm(mi,mj) then (i,j) can be discarded
  }
  return false;
}

//    //



inline void queue_insert(GpiList& Q, int i, int j, int deg_lcm, GpiList const& lcm)
{
  GpiList data = {i, j, deg_lcm, lcm};
  if(Q.size()==0)
  {
    Q.push_back(data);
    return;
  }
  int k=0;
  for(GpiList::iterator it=Q.begin(); it!=Q.end(); ++it)
  {
    k++; //std::cout << "    k=" << k << std::endl;
    GpiList entry = get_list(*it);
    if(dp_larger_equal(lcm, get_list(entry.back()), deg_lcm, boost::get<int>(*std::next(entry.begin(),2))))
    {
      Q.insert(it, data);
      return;
    }
  }
  Q.push_back(data);
}
/*
inline void queue_insert(GpiList& Q, GpiList const& data)
{
  if(Q.size()==0)
  {
    Q.push_back(data);
    return;
  }
  int i=0;
  for(GpiList::iterator it=Q.begin(); it!=Q.end(); ++it)
  {
    i++; //std::cout << "    i=" << i << std::endl;
    if(dp_larger_equal(data.back(), get_list(*it).back()))
    {
      Q.insert(it, data);
      return;
    }
  }
  Q.push_back(data);
}
*/

inline void queue_remove(GpiList& Q, int i, int j) // order of i,j does not matter! // SLOW! (O(r^2))
{
  if(i<j) {queue_remove(Q,j,i);}
  for(GpiList::iterator it=Q.begin(); it!=Q.end(); ++it)
  {
    if(boost::get<int>(get_list(*it).front())==i && boost::get<int>(*std::next(get_list(*it).begin()))==j)
    {
      Q.erase(it);
      return;
    }
  }
  throw std::runtime_error ("tried to remove ("+std::to_string(i)+","+std::to_string(j)+")-element, but it was not was not in queue");
}

/*
inline void update_queue(GpiList& Q, GpiMap& bookkeeping, GpiList& Qback) // remove all finished (i,j) from Q
{
  int k=1;
  int r = Q.size();
  for (GpiList::iterator Qk=Q.begin(); k<=r; k++)
  {
    int i = boost::get<int>(get_list(*Qk).front());
    int j = boost::get<int>(*std::next(get_list(*Qk).begin()));
    GpiVariant tmpvar = (bookkeeping[GpiSet({i,j})]);
    int state = boost::get<int>(get_list(tmpvar).front());
    if(state!=0 && state!=STARTED)
    {
      Q.erase(Qk++);
    }
    else
    {
      ++Qk;
    }
  }
  if (Q.size()>0)
    Qback = GpiList({get_list(Q.back()).front(), *std::next(get_list(Q.back()).begin())});
}
*/

inline void queue_delete_i_j(GpiList& Q, int i, int j) // remove index (i,j) from Q
{
  for (GpiList::iterator Qk=Q.begin(); Qk!=Q.end(); )
  {
    int ii = boost::get<int>(get_list(*Qk).front());
    int jj = boost::get<int>(*std::next(get_list(*Qk).begin()));
    if(ii==i && jj==j)
    {
      Qk = Q.erase(Qk);
      break;
    }
    else
    {
      ++Qk;
    }
  }
}

inline void queue_delete_i(GpiList& Q, int i) // remove indices (i,j) and (j,i) from Q (for all j)
{
  for (GpiList::iterator Qk=Q.begin(); Qk!=Q.end(); )
  {
    int ii = boost::get<int>(get_list(*Qk).front());
    int jj = boost::get<int>(*std::next(get_list(*Qk).begin()));
    if(ii==i || jj==i)
    {
      Qk = Q.erase(Qk);
    }
    else
    {
      ++Qk;
    }
  }
}



NO_NAME_MANGLING

void singular_buchberger_compute(std::string const& singular_library_name,
																 std::string const& singular_function_name,
															 	 std::string const& base_filename,
																 std::vector<boost::variant<long,std::string,GpiList>> const& args_read,
																 std::vector<boost::variant<long,std::string,GpiList>> const& args_in,
																 std::vector<boost::variant<long*,std::string*,GpiList*>> &args_inout,
																 std::vector<boost::variant<long*,std::string*,GpiList*>> &out,
																 std::vector<GpiList*> &out_many,
															 	 bool delete_files=true,
                                 bool silent=true);

NO_NAME_MANGLING

void singular_buchberger_compute_NF(std::string const& singular_library_name,
                                    std::string const& singular_function_name,
                                    std::string const& base_filename,
                                    std::list<poly> const& generators,
                                    int r,
                                    /*[[maybe_unused]] GpiList const& M,*/
                                    int Qback_i,
                                    int Qback_j,
                                    int index_i,
                                    int index_j,
                                    int old_r,
                                    long time_offset,
                                    GpiMap* runtime,
                                    GpiList* started_indices_out,
                                    GpiList* finished_indices,
                                    GpiList* NF,
                                    [[maybe_unused]] bool delete_files,
                                    bool silent );

NO_NAME_MANGLING

std::vector<std::vector<int>> singular_buchberger_get_M_and_init_F( [[maybe_unused]] std::string const& singular_library_name,
                                                                    [[maybe_unused]] std::string const& base_filename,
                                                                    [[maybe_unused]] std::string const& input,
                                                                    long time_offset,
                                                                    [[maybe_unused]] GpiMap* runtime );

NO_NAME_MANGLING

poly read_generator (std::string const& base_filename, int k, std::vector<int> permutation);

NO_NAME_MANGLING

void singular_buchberger_reduce_GB (std::string const& base_filename,
                                    std::string const& singular_library_name,
                                    std::list<poly> const& generators,
                                    long needed_indices,
                                    int current_index,
                                    int final_r,
                                    long redSB,
                                    long time_offset,
                                    GpiMap* runtime);
