import json
import urllib.request
import time
import re
import sys

def extract_solution_code(full_code):
    lines = full_code.split('\n')
    solution_lines = []
    in_main = False
    
    for line in lines:
        if re.match(r'^\s*int\s+main\s*\(', line):
            in_main = True
            break
        if not in_main:
            solution_lines.append(line)
            
    code = '\n'.join(solution_lines)
    # Strip struct ListNode redefinition for LeetCode
    code = re.sub(r'struct\s+ListNode\s*\{[\s\S]*?\};', '', code)
    return code

def submit_solution(slug, question_id, code_path):
    try:
        with open('leetcode_config.json', 'r', encoding='utf-8') as f:
            cfg = json.load(f)
    except Exception as e:
        print(f"Error loading leetcode_config.json: {e}")
        return None

    with open(code_path, 'r', encoding='utf-8') as f:
        full_code = f.read()

    typed_code = extract_solution_code(full_code)

    payload = {
        'lang': 'c',
        'question_id': str(question_id),
        'typed_code': typed_code
    }
    headers = {
        'Content-Type': 'application/json',
        'User-Agent': 'Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/120.0.0.0 Safari/537.36',
        'x-csrftoken': cfg['csrftoken'],
        'Referer': f'https://leetcode.com/problems/{slug}/',
        'Origin': 'https://leetcode.com',
        'Cookie': f"LEETCODE_SESSION={cfg['session']}; csrftoken={cfg['csrftoken']}"
    }

    print(f"Submitting {slug} (Q#{question_id})...")
    req = urllib.request.Request(
        f'https://leetcode.com/problems/{slug}/submit/',
        data=json.dumps(payload).encode('utf-8'),
        headers=headers
    )

    try:
        res = json.load(urllib.request.urlopen(req))
        sub_id = res.get('submission_id')
        print(f"Submission ID: {sub_id}")
        if not sub_id:
            print('Error in response:', res)
            return None

        for attempt in range(12):
            time.sleep(2)
            check_req = urllib.request.Request(
                f'https://leetcode.com/submissions/detail/{sub_id}/check/',
                headers=headers
            )
            chk = json.load(urllib.request.urlopen(check_req))
            state = chk.get('state')
            if state == 'SUCCESS':
                msg = chk.get('status_msg')
                runtime = chk.get('status_runtime')
                memory = chk.get('status_memory')
                print(f"Status: {msg}")
                print(f"Runtime: {runtime}")
                print(f"Memory: {memory}")
                if msg != 'Accepted':
                    if chk.get('compile_error'):
                        print('Compile Error:\n', chk.get('compile_error'))
                    if chk.get('runtime_error'):
                        print('Runtime Error:\n', chk.get('runtime_error'))
                    if chk.get('input'):
                        print('Failed Testcase:\n', chk.get('input'))
                else:
                    correct = chk.get('total_correct')
                    total = chk.get('total_testcases')
                    print(f"Passed: {correct}/{total}")
                return chk
            else:
                print(f"Waiting for result... ({state})")
    except Exception as e:
        print(f"Submission error: {e}")
        return None

if __name__ == '__main__':
    import glob
    problem = sys.argv[1].lower() if len(sys.argv) > 1 else 'p1'
    day_matches = glob.glob('Day112*')
    day_dir = day_matches[0] if day_matches else 'Day112'
    
    mapping = {
        'p1': ('single-number', 136, f'{day_dir}/p1.c'),
        'p2': ('merge-two-sorted-lists', 21, f'{day_dir}/p2.c'),
        'p3': ('linked-list-cycle', 141, f'{day_dir}/p3.c')
    }

    if problem in mapping:
        slug, qid, path = mapping[problem]
        submit_solution(slug, qid, path)
    else:
        print(f"Unknown problem '{problem}'. Available: p1, p2, p3")
