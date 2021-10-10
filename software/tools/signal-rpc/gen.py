import json
import sys
import os
import argparse
import jinja2

if __name__ == '__main__':
    parser = argparse.ArgumentParser()
    parser.add_argument('--signals', required=True)
    parser.add_argument('--output-dir', required=True)
    args = parser.parse_args(sys.argv[1:])

    with open(args.signals, 'r') as signals_json:
        signals = json.load(signals_json)

        engine = jinja2.Environment(loader=
                    jinja2.FileSystemLoader(os.path.join(os.path.dirname(__file__), 'templates')))
        header_template = engine.get_template('signal_api.h.jinja2')
        header_content = header_template.render(signals)
        header_path = os.path.join(args.output_dir, 'signal_api.h')

        source_template = engine.get_template('signals.c.jinja2')
        source_content = source_template.render(signals)
        source_path = os.path.join(args.output_dir, 'signals.c')

        os.makedirs(args.output_dir, exist_ok=True)
        with open(header_path, 'w+') as header, open(source_path, 'w+') as source:
            header.writelines(header_content)
            source.writelines(source_content)
